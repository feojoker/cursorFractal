import express from 'express';
import cors from 'cors';
import { fileURLToPath } from 'url';
import { dirname, join } from 'path';
import fs from 'fs';
import { spawn } from 'child_process';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const app = express();
const PORT = 3005;

// Rate limiting for C++ generation
let isGenerating = false;

// Middleware
app.use(cors());
app.use(express.json());

// Load the C++ generated fractal data
let cppFractalData = null;

try {
    const jsonPath = join(__dirname, '..', 'fractal_data.json');
    const jsonData = fs.readFileSync(jsonPath, 'utf8');
    cppFractalData = JSON.parse(jsonData);
    console.log('✅ Loaded C++ fractal data:', {
        triangles: cppFractalData.metadata.triangleCount,
        vertices: cppFractalData.metadata.vertexCount,
        iterations: cppFractalData.metadata.iterations,
        gridSize: cppFractalData.metadata.gridSize
    });
} catch (error) {
    console.error('❌ Failed to load C++ fractal data:', error.message);
    console.log('📝 Make sure to run: ./morphosis -j to generate fractal_data.json');
}

// API endpoint for fractal generation
app.post('/api/fractal', async (req, res) => {
    try {
        const { iterations = 6, c = [-0.2, 0.8, 0.0, 0.0], gridSize = 60 } = req.body;

        console.log('🎯 Serving fractal data with params:', { iterations, c, gridSize });

        // Check if parameters match the C++ data
        const cppParams = cppFractalData ? {
            iterations: cppFractalData.metadata.iterations,
            c: cppFractalData.metadata.juliaC,
            gridSize: cppFractalData.metadata.gridSize
        } : null;

        const paramsMatch = cppParams &&
            iterations === cppParams.iterations &&
            JSON.stringify(c) === JSON.stringify(cppParams.c) &&
            gridSize === cppParams.gridSize;

        if (cppFractalData && paramsMatch) {
            // Serve the real C++ generated data when parameters match
            console.log('✅ Using C++ data (parameters match)');
            res.json({
                success: true,
                vertices: cppFractalData.vertices,
                indices: cppFractalData.indices,
                triangleCount: cppFractalData.metadata.triangleCount,
                vertexCount: cppFractalData.metadata.vertexCount,
                metadata: {
                    iterations: cppFractalData.metadata.iterations,
                    c: cppFractalData.metadata.juliaC,
                    gridSize: cppFractalData.metadata.gridSize,
                    stepSize: cppFractalData.metadata.stepSize,
                    source: 'C++ Generated',
                    generatedAt: new Date().toISOString()
                }
            });
        } else {
            // Check if already generating
            if (isGenerating) {
                console.log('⏳ C++ generation already in progress, using fallback...');
                const fallbackData = generateSimpleFractal(iterations, c, Math.min(gridSize, 30));
                res.json({
                    success: true,
                    vertices: fallbackData.vertices,
                    indices: fallbackData.indices,
                    triangleCount: fallbackData.triangleCount,
                    vertexCount: fallbackData.vertexCount,
                    metadata: {
                        iterations,
                        c,
                        gridSize: Math.min(gridSize, 30),
                        source: 'JavaScript Fallback (Rate Limited)',
                        generatedAt: new Date().toISOString(),
                        note: 'C++ generation in progress, using fallback'
                    }
                });
                return;
            }

            // Generate new fractal data using C++ program
            console.log('🔄 Generating new fractal with C++ program...');
            isGenerating = true;

            try {
                const fractalData = await generateFractalWithCpp(iterations, c, gridSize);
                res.json({
                    success: true,
                    vertices: fractalData.vertices,
                    indices: fractalData.indices,
                    triangleCount: fractalData.triangleCount,
                    vertexCount: fractalData.vertexCount,
                    metadata: {
                        iterations,
                        c,
                        gridSize,
                        source: 'C++ Generated (Dynamic)',
                        generatedAt: new Date().toISOString()
                    }
                });
                isGenerating = false;
            } catch (error) {
                console.error('❌ C++ generation failed:', error.message);
                console.log('🔄 Falling back to simplified JavaScript generation...');

                try {
                    const fallbackData = generateSimpleFractal(iterations, c, Math.min(gridSize, 30));
                    res.json({
                        success: true,
                        vertices: fallbackData.vertices,
                        indices: fallbackData.indices,
                        triangleCount: fallbackData.triangleCount,
                        vertexCount: fallbackData.vertexCount,
                        metadata: {
                            iterations,
                            c,
                            gridSize: Math.min(gridSize, 30),
                            source: 'JavaScript Fallback (Simplified)',
                            generatedAt: new Date().toISOString(),
                            note: 'C++ generation failed, using simplified fallback'
                        }
                    });
                } catch (fallbackError) {
                    console.error('❌ Fallback generation also failed:', fallbackError.message);
                    res.status(500).json({ error: 'Failed to generate fractal data' });
                }
                isGenerating = false;
            }
        }
    } catch (error) {
        console.error('❌ Fractal generation error:', error);
        res.status(500).json({ error: error.message });
    }
});

// Generate fractal data using C++ program
async function generateFractalWithCpp(iterations, c, gridSize) {
    return new Promise((resolve, reject) => {
        const cppPath = join(__dirname, '..', 'morphosis');
        const stepSize = 0.05; // Fixed step size for consistency

        console.log(`🚀 Running C++ program with params: iterations=${iterations}, c=[${c.join(', ')}], gridSize=${gridSize}`);

        const args = ['-j', stepSize.toString(), c[0].toString(), c[1].toString(), c[2].toString(), c[3].toString(), iterations.toString()];

        const cppProcess = spawn(cppPath, args, {
            cwd: join(__dirname, '..'),
            stdio: ['ignore', 'pipe', 'pipe']
        });

        // Set a timeout for the C++ process
        const timeout = setTimeout(() => {
            cppProcess.kill('SIGTERM');
            reject(new Error('C++ program timed out after 30 seconds'));
        }, 30000);

        let stderr = '';

        cppProcess.stdout.on('data', (data) => {
            // stdout can be used for debugging if needed
            console.log('C++ stdout:', data.toString());
        });

        cppProcess.stderr.on('data', (data) => {
            stderr += data.toString();
        });

        cppProcess.on('close', (code) => {
            clearTimeout(timeout);

            if (code !== 0) {
                console.error('❌ C++ program failed:', stderr);
                reject(new Error(`C++ program exited with code ${code}: ${stderr}`));
                return;
            }

            try {
                // Read the generated JSON file
                const jsonPath = join(__dirname, '..', 'fractal_data.json');
                const jsonData = fs.readFileSync(jsonPath, 'utf8');
                const fractalData = JSON.parse(jsonData);

                console.log(`✅ C++ generation complete: ${fractalData.metadata.triangleCount} triangles`);
                resolve(fractalData);
            } catch (error) {
                console.error('❌ Failed to read generated JSON:', error.message);
                reject(new Error(`Failed to read generated JSON: ${error.message}`));
            }
        });

        cppProcess.on('error', (error) => {
            console.error('❌ Failed to start C++ program:', error.message);
            reject(new Error(`Failed to start C++ program: ${error.message}`));
        });
    });
}

// Simple fallback fractal generation for when C++ fails
function generateSimpleFractal(iterations, c, gridSize) {
    const vertices = [];
    const indices = [];

    console.log(`📊 Generating simple fractal: iterations=${iterations}, gridSize=${gridSize}`);

    // Create a simple sphere-like fractal
    const radius = 1.0;
    const segments = Math.min(gridSize, 20);

    for (let i = 0; i < segments; i++) {
        for (let j = 0; j < segments; j++) {
            const u = (i / segments) * Math.PI * 2;
            const v = (j / segments) * Math.PI;

            const x = radius * Math.sin(v) * Math.cos(u);
            const y = radius * Math.sin(v) * Math.sin(u);
            const z = radius * Math.cos(v);

            // Add some fractal-like variation
            const variation = Math.sin(x * iterations) * Math.cos(y * iterations) * 0.1;

            vertices.push(x + variation, y + variation, z + variation);
        }
    }

    // Create triangles
    for (let i = 0; i < segments - 1; i++) {
        for (let j = 0; j < segments - 1; j++) {
            const a = i * segments + j;
            const b = i * segments + (j + 1);
            const c = (i + 1) * segments + j;
            const d = (i + 1) * segments + (j + 1);

            // Two triangles per quad
            indices.push(a, b, c);
            indices.push(b, d, c);
        }
    }

    return {
        vertices,
        indices,
        triangleCount: indices.length / 3,
        vertexCount: vertices.length / 3
    };
}

app.listen(PORT, () => {
    console.log(`🚀 Server running on http://localhost:${PORT}`);
    console.log(`📊 C++ fractal data: ${cppFractalData ? '✅ Loaded' : '❌ Not available'}`);
});

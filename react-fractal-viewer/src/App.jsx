import React, { useState, useEffect } from 'react'
import { Canvas } from '@react-three/fiber'
import { OrbitControls, Stats } from '@react-three/drei'
import FractalMesh from './components/FractalMesh'
import ControlPanel from './components/ControlPanel'
import LoadingOverlay from './components/LoadingOverlay'
import './App.css'

// Error Boundary Component
class ErrorBoundary extends React.Component {
  constructor(props) {
    super(props)
    this.state = { hasError: false, error: null }
  }

  static getDerivedStateFromError(error) {
    return { hasError: true, error }
  }

  componentDidCatch(error, errorInfo) {
    console.error('❌ ErrorBoundary caught an error:', error, errorInfo)
  }

  render() {
    if (this.state.hasError) {
      return (
        <div className="error-display" style={{ padding: '20px', color: 'red', background: '#ffe6e6', border: '1px solid red', borderRadius: '4px', margin: '10px' }}>
          <h3>❌ Something went wrong in the Control Panel</h3>
          <p>Error: {this.state.error?.message || 'Unknown error'}</p>
          <button onClick={() => this.setState({ hasError: false, error: null })}>
            Try Again
          </button>
        </div>
      )
    }

    return this.props.children
  }
}

function App() {
  const [fractalParams, setFractalParams] = useState({
    iterations: 6,
    gridSize: 60,
    c: [-0.2, 0.8, 0.0, 0.0]
  })

  const [fractalData, setFractalData] = useState(null)
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState(null)
  const [renderMode, setRenderMode] = useState('wireframe')
  const [autoRotate, setAutoRotate] = useState(false)
  const [stats, setStats] = useState({ triangles: 0, vertices: 0, fps: 0 })

  // Generate fractal data
  const generateFractal = async (params = fractalParams) => {
    setLoading(true)
    setError(null)

    try {
      console.log('🔄 Generating fractal with params:', params)

      const response = await fetch('/api/fractal', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(params)
      })

      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`)
      }

      const data = await response.json()

      console.log('✅ Fractal data received:', {
        triangles: data.triangleCount,
        vertices: data.vertexCount
      })

      if (data.vertices && data.vertices.length > 0) {
        setFractalData(data)

        // Calculate stats from actual data arrays
        const vertexCount = data.vertices.length / 3; // 3 coordinates per vertex
        const triangleCount = data.indices ? data.indices.length / 3 : vertexCount / 3; // 3 indices per triangle

        setStats({
          triangles: triangleCount,
          vertices: vertexCount,
          fps: stats.fps || 0
        })
      } else {
        throw new Error('No fractal geometry generated')
      }

    } catch (err) {
      console.error('❌ Error generating fractal:', err)
      setError(err.message)
    } finally {
      setLoading(false)
    }
  }

  // Update parameters and regenerate
  const updateParams = (newParams) => {
    const updatedParams = { ...fractalParams, ...newParams }
    setFractalParams(updatedParams)
    generateFractal(updatedParams)
  }

  // Apply presets
  const applyPreset = (preset) => {
    let newParams = { ...fractalParams }

    switch (preset) {
      case 'default':
        newParams = { ...newParams, c: [-0.4, 0.6, 0, 0], iterations: 8 }
        break
      case 'spiral':
        newParams = { ...newParams, c: [0.3, 0.5, 0.2, 0.1], iterations: 12 }
        break
      case 'quaternion':
        newParams = { ...newParams, c: [0.18, 0, 0, 0.78], iterations: 10 }
        break
    }

    console.log(`🎭 Applying preset: ${preset}`, newParams)
    updateParams(newParams)
  }

  // Generate initial fractal
  useEffect(() => {
    generateFractal()
  }, [])

  return (
    <div className="app">
      <ErrorBoundary>
        <ControlPanel
          params={fractalParams}
          onParamsChange={updateParams}
          onPresetApply={applyPreset}
          onRenderModeChange={setRenderMode}
          onAutoRotateChange={setAutoRotate}
          renderMode={renderMode}
          autoRotate={autoRotate}
          stats={stats}
          error={error}
          onRegenerate={() => generateFractal()}
        />
      </ErrorBoundary>

      <div className="canvas-container">
        <Canvas
          camera={{ position: [3, 3, 3], fov: 75 }}
          style={{ background: 'linear-gradient(135deg, #1e1e1e, #2d2d30)' }}
        >
          <ambientLight intensity={0.6} />
          <directionalLight position={[10, 10, 5]} intensity={0.8} />

          <FractalMesh
            data={fractalData}
            wireframe={renderMode === 'wireframe'}
          />

          <OrbitControls
            enablePan={true}
            enableZoom={true}
            enableRotate={true}
            autoRotate={autoRotate}
            autoRotateSpeed={2}
          />

          <Stats />
        </Canvas>

        {loading && <LoadingOverlay />}
      </div>
    </div>
  )
}

export default App

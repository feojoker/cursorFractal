import React, { useState, useEffect, useRef } from 'react'
import { Canvas } from '@react-three/fiber'
import { OrbitControls, Stats } from '@react-three/drei'
import FractalMesh from './components/FractalMesh'
import ControlPanel from './components/ControlPanel'
import LoadingOverlay from './components/LoadingOverlay'
import './App.css'

// Transition Loading Component
function TransitionLoader({ backgroundColor }) {
  return (
    <div style={{
      display: 'flex',
      alignItems: 'center',
      justifyContent: 'center',
      height: '100%',
      background: `linear-gradient(135deg, ${backgroundColor}, #2d2d30)`,
      color: '#4CAF50',
      fontSize: '18px',
      fontWeight: '600'
    }}>
      <div style={{ display: 'flex', alignItems: 'center', gap: '16px' }}>
        <div style={{
          width: '24px',
          height: '24px',
          border: '3px solid rgba(76, 175, 80, 0.3)',
          borderRadius: '50%',
          borderTopColor: '#4CAF50',
          animation: 'spin 0.8s ease-in-out infinite'
        }} />
        Adjusting view...
      </div>
    </div>
  )
}

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
  const [toolbarVisible, setToolbarVisible] = useState(true)
  const [isTransitioning, setIsTransitioning] = useState(false)
  const [colors, setColors] = useState({
    lineColor: '#4CAF50',
    backgroundColor: '#1e1e1e'
  })

  // Request cancellation and debouncing
  const abortControllerRef = useRef(null)
  const debounceTimeoutRef = useRef(null)

  // Generate fractal data with race condition protection
  const generateFractal = async (params = fractalParams) => {
    // Cancel any existing request
    if (abortControllerRef.current) {
      abortControllerRef.current.abort()
    }

    // Clear existing debounce timeout
    if (debounceTimeoutRef.current) {
      clearTimeout(debounceTimeoutRef.current)
    }

    // Debounce rapid calls (wait 300ms)
    return new Promise((resolve) => {
      debounceTimeoutRef.current = setTimeout(async () => {
        await performGeneration(params)
        resolve()
      }, 300)
    })
  }

  const performGeneration = async (params) => {
    // Create new abort controller for this request
    abortControllerRef.current = new AbortController()

    setLoading(true)
    setError(null)

    try {
      console.log('🔄 Generating fractal with params:', params)

      const response = await fetch('/api/fractal', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(params),
        signal: abortControllerRef.current.signal
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
      // Don't show error for aborted requests (user changed parameters)
      if (err.name === 'AbortError') {
        console.log('🚫 Request cancelled (user changed parameters)')
        return
      }

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

  // Handle color changes
  const updateColors = (newColors) => {
    setColors(prev => ({ ...prev, ...newColors }))
  }

  // Toggle toolbar visibility
  const toggleToolbar = () => {
    setIsTransitioning(true)
    setToolbarVisible(!toolbarVisible)

    // Wait for CSS transition to complete (0.4s) before showing the scene
    setTimeout(() => {
      setIsTransitioning(false)
    }, 450) // Slightly longer than CSS transition to ensure it's complete
  }

  // Generate initial fractal
  useEffect(() => {
    generateFractal()

    // Cleanup on unmount
    return () => {
      if (abortControllerRef.current) {
        abortControllerRef.current.abort()
      }
      if (debounceTimeoutRef.current) {
        clearTimeout(debounceTimeoutRef.current)
      }
    }
  }, []) // eslint-disable-line react-hooks/exhaustive-deps

  return (
    <div className="app">
      {/* Toggle Button */}
      <button
        className={`toolbar-toggle ${toolbarVisible ? 'visible' : 'hidden'}`}
        onClick={toggleToolbar}
        title={toolbarVisible ? 'Hide Toolbar' : 'Show Toolbar'}
      >
        {toolbarVisible ? '◀' : '▶'}
      </button>

      {/* Control Panel */}
      <div className={`control-panel-wrapper ${toolbarVisible ? 'visible' : 'hidden'}`}>
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
            colors={colors}
            onColorsChange={updateColors}
          />
        </ErrorBoundary>
      </div>

      <div className={`canvas-container ${!toolbarVisible ? 'full-width' : ''}`}>
        {isTransitioning ? (
          <TransitionLoader backgroundColor={colors.backgroundColor} />
        ) : (
          <Canvas
            camera={{ position: [3, 3, 3], fov: 75 }}
            style={{ background: `linear-gradient(135deg, ${colors.backgroundColor}, #2d2d30)` }}
          >
            <ambientLight intensity={0.6} />
            <directionalLight position={[10, 10, 5]} intensity={0.8} />

            <FractalMesh
              data={fractalData}
              wireframe={renderMode === 'wireframe'}
              lineColor={colors.lineColor}
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
        )}

        {loading && <LoadingOverlay />}
      </div>
    </div>
  )
}

export default App

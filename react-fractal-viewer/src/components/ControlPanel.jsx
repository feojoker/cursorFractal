import { useState } from 'react'

function ControlPanel({
  params,
  onParamsChange,
  onPresetApply,
  onRenderModeChange,
  onAutoRotateChange,
  renderMode,
  autoRotate,
  stats,
  error,
  onRegenerate
}) {
  const [localParams, setLocalParams] = useState(params)

  // Add error boundary and validation
  if (!params || !params.c || !Array.isArray(params.c) || params.c.length !== 4) {
    console.error('❌ Invalid params in ControlPanel:', params)
    return <div className="error-display">❌ Invalid parameters</div>
  }

  if (!stats || typeof stats.triangles !== 'number' || typeof stats.vertices !== 'number') {
    console.error('❌ Invalid stats in ControlPanel:', stats)
    return <div className="error-display">❌ Invalid statistics</div>
  }

  const handleParamChange = (key, value) => {
    const newParams = { ...localParams, [key]: value }
    setLocalParams(newParams)
    onParamsChange({ [key]: value })
  }

  const handleQuaternionChange = (index, value) => {
    const newC = [...localParams.c]
    newC[index] = parseFloat(value)
    const newParams = { ...localParams, c: newC }
    setLocalParams(newParams)
    onParamsChange({ c: newC })
  }

  const handlePreset = (preset) => {
    onPresetApply(preset)
    // Update local state to match preset
    setTimeout(() => {
      setLocalParams(params)
    }, 100)
  }

  return (
    <div className="control-panel">
      <div className="panel-title">
        🌀 4D Julia Fractal
      </div>

      {/* Fractal Parameters */}
      <div className="panel-section">
        <div className="section-title">📐 Fractal Parameters</div>

        <div className="control-group">
          <label className="control-label">Iterations: {localParams.iterations}</label>
          <input
            type="range"
            className="slider"
            min="1"
            max="20"
            value={localParams.iterations}
            onChange={(e) => handleParamChange('iterations', parseInt(e.target.value))}
          />
          <div className="value-display">{localParams.iterations}</div>
        </div>

        <div className="control-group">
          <label className="control-label">Grid Resolution: {localParams.gridSize}</label>
          <input
            type="range"
            className="slider"
            min="16"
            max="64"
            value={localParams.gridSize}
            onChange={(e) => handleParamChange('gridSize', parseInt(e.target.value))}
          />
          <div className="value-display">{localParams.gridSize}</div>
        </div>
      </div>

      {/* Quaternion Parameters */}
      <div className="panel-section">
        <div className="section-title">🎯 Quaternion (c = a + bi + cj + dk)</div>

        {['Real (a)', 'Imaginary i (b)', 'Imaginary j (c)', 'Imaginary k (d)'].map((label, index) => (
          <div key={index} className="control-group">
            <label className="control-label">{label}: {localParams.c[index].toFixed(2)}</label>
            <input
              type="range"
              className="slider"
              min="-2"
              max="2"
              step="0.01"
              value={localParams.c[index]}
              onChange={(e) => handleQuaternionChange(index, e.target.value)}
            />
            <div className="value-display">{localParams.c[index].toFixed(2)}</div>
          </div>
        ))}
      </div>

      {/* Rendering Options */}
      <div className="panel-section">
        <div className="section-title">🎨 Rendering</div>

        <div className="checkbox-group">
          <input
            type="checkbox"
            className="checkbox"
            id="wireframe"
            checked={renderMode === 'wireframe'}
            onChange={(e) => onRenderModeChange(e.target.checked ? 'wireframe' : 'solid')}
          />
          <label htmlFor="wireframe" className="checkbox-label">Wireframe Mode</label>
        </div>

        <div className="checkbox-group">
          <input
            type="checkbox"
            className="checkbox"
            id="autoRotate"
            checked={autoRotate}
            onChange={(e) => onAutoRotateChange(e.target.checked)}
          />
          <label htmlFor="autoRotate" className="checkbox-label">Auto Rotate</label>
        </div>
      </div>

      {/* Presets */}
      <div className="panel-section">
        <div className="section-title">🎭 Presets</div>
        <div className="preset-buttons">
          <button className="button secondary" onClick={() => handlePreset('default')}>
            Default
          </button>
          <button className="button secondary" onClick={() => handlePreset('spiral')}>
            Spiral
          </button>
          <button className="button secondary" onClick={() => handlePreset('quaternion')}>
            4D
          </button>
        </div>
      </div>

      {/* Actions */}
      <div className="panel-section">
        <div className="section-title">⚡ Actions</div>
        <button className="button" onClick={onRegenerate}>
          🔄 Regenerate
        </button>
      </div>

      {/* Stats */}
      <div className="panel-section">
        <div className="section-title">📊 Statistics</div>
        <div className="stats-display">
          <div style={{ fontSize: '12px', lineHeight: '1.4' }}>
            <strong>Triangles:</strong> {stats.triangles.toLocaleString()}<br/>
            <strong>Vertices:</strong> {stats.vertices.toLocaleString()}<br/>
            <strong>Complexity:</strong> {stats.triangles > 50000 ? 'High' : stats.triangles > 10000 ? 'Medium' : 'Low'}<br/>
            <strong>Status:</strong> {stats.triangles > 0 ? '✅ Ready' : '⏳ Generating...'}
          </div>
        </div>
      </div>

      {/* Error Display */}
      {error && (
        <div className="error-display">
          ❌ Error: {error}
        </div>
      )}

      {/* Instructions */}
      <div className="panel-section">
        <div className="section-title">🎮 Controls</div>
        <div style={{ fontSize: '12px', color: '#888', lineHeight: '1.4' }}>
          • <strong>Mouse:</strong> Rotate, pan, zoom<br/>
          • <strong>Sliders:</strong> Real-time updates<br/>
          • <strong>Presets:</strong> Complex, Spiral, 4D<br/>
          • <strong>Wireframe:</strong> Toggle rendering mode
        </div>
      </div>
    </div>
  )
}

export default ControlPanel

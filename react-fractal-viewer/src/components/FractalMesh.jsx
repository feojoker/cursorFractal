import { useRef, useEffect, useMemo } from 'react'
import { useFrame } from '@react-three/fiber'
import * as THREE from 'three'

function FractalMesh({ data, wireframe = true }) {
  const meshRef = useRef()

  // Create geometry from fractal data
  const geometry = useMemo(() => {
    if (!data || !data.vertices || data.vertices.length === 0) {
      return new THREE.BufferGeometry()
    }

    console.log('🔺 Creating geometry:', {
      vertices: data.vertices.length,
      indices: data.indices.length,
      triangles: data.triangleCount
    })

    const geo = new THREE.BufferGeometry()

    // Set vertices
    const positions = new Float32Array(data.vertices)
    geo.setAttribute('position', new THREE.BufferAttribute(positions, 3))

    // Set indices if available
    if (data.indices && data.indices.length > 0) {
      geo.setIndex(data.indices)
    }

    // Compute normals for proper lighting
    geo.computeVertexNormals()

    // Center the geometry
    geo.computeBoundingBox()
    const center = new THREE.Vector3()
    geo.boundingBox.getCenter(center)
    geo.translate(-center.x, -center.y, -center.z)

    return geo
  }, [data])

  // Create material
  const material = useMemo(() => {
    return new THREE.MeshLambertMaterial({
      color: new THREE.Color('#4CAF50'),
      wireframe: wireframe,
      transparent: false,
      opacity: 1.0,
      side: THREE.DoubleSide
    })
  }, [wireframe])

  // Update material when wireframe changes
  useEffect(() => {
    if (meshRef.current) {
      meshRef.current.material.wireframe = wireframe
    }
  }, [wireframe])

  // Auto-rotation animation (optional)
  useFrame((state) => {
    if (meshRef.current) {
      // Subtle breathing animation
      const time = state.clock.getElapsedTime()
      meshRef.current.scale.setScalar(1 + Math.sin(time * 0.5) * 0.05)
    }
  })

  if (!data || !data.vertices || data.vertices.length === 0) {
    return null
  }

  return (
    <mesh ref={meshRef} geometry={geometry} material={material} />
  )
}

export default FractalMesh

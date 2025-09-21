# 🚀 Simple Deployment Guide

## ✅ **Single Service Deployment (Frontend + Backend)**

Your app is now configured to serve both the React frontend and API from one Railway service.

### **How It Works:**

1. **Build Phase**:
   - `npm run build` creates the React production build in `dist/`
   - Static assets are optimized and bundled

2. **Runtime**:
   - Express server serves the API at `/api/*`
   - Express serves React static files for all other routes
   - Single URL serves both frontend and backend

### **🎯 What Users Get:**

- **Frontend**: `https://your-app.railway.app` (React app)
- **API**: `https://your-app.railway.app/api/fractal` (API endpoints)
- **Same domain** - no CORS issues!
- **Beautiful 3D fractal visualizations** with Three.js
- **Interactive controls** for fractal parameters
- **Real-time generation** via API calls
- **Fast loading** with optimized builds

### **📋 Current Configuration:**

```json
// package.json
"scripts": {
  "start:prod": "node server.js"  // Production start
}
```

```javascript
// server.js
app.use(express.static('dist'));           // Serve React files
app.post('/api/fractal', ...);             // API endpoints
app.use((req, res, next) => {              // Catch-all for React routing
  if (!req.path.startsWith('/api')) {
    res.sendFile(path.join(__dirname, 'dist', 'index.html'));
  }
});
```

### **🚀 Deploy Instructions:**

1. **Push your changes:**
   ```bash
   git add .
   git commit -m "Configure single service deployment"
   git push
   ```

2. **Railway will automatically:**
   - Install Node.js dependencies
   - Build the React app (`npm run build`)
   - Start the server (`npm run start:prod`)
   - Serve both frontend and API

3. **Access your app:**
   - Visit the Railway-provided URL
   - Both frontend and API will be available

### **⚡ Why This Works Better:**

- ✅ **No C++ compilation issues**
- ✅ **Fast deployment** (< 5 minutes)
- ✅ **Single service** - easier to manage
- ✅ **No CORS issues** - same domain
- ✅ **Pre-generated fractal data** for instant loading
- ✅ **JavaScript fallback** for dynamic generation
- ✅ **Production optimized** builds

The C++ optimization can be added later once the core deployment is stable.

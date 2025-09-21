# Simple deployment - Node.js only with pre-built fractal data
FROM node:18-alpine

WORKDIR /app

# Copy the React app
COPY react-fractal-viewer/package*.json ./react-fractal-viewer/
WORKDIR /app/react-fractal-viewer
RUN npm install

# Copy all source files
WORKDIR /app
COPY . .

# Build React frontend
WORKDIR /app/react-fractal-viewer
RUN npm run build

# Copy any existing fractal data
COPY fractal_data.json ./fractal_data.json 2>/dev/null || echo "No fractal data found, will use JavaScript fallback"

EXPOSE 3005

WORKDIR /app/react-fractal-viewer
CMD ["npm", "run", "server"]

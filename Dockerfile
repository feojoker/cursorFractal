# Simple deployment - Node.js only with pre-built fractal data
FROM node:18-alpine

WORKDIR /app

# Copy the React app
COPY react-fractal-viewer/package*.json ./react-fractal-viewer/
WORKDIR /app/react-fractal-viewer
RUN npm install

# Copy all source files (including fractal_data.json if it exists)
WORKDIR /app
COPY . .

# Build React frontend
WORKDIR /app/react-fractal-viewer
RUN npm run build

EXPOSE 3005

WORKDIR /app/react-fractal-viewer
CMD ["npm", "run", "start:prod"]

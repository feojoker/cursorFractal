# Full deployment with C++ compilation and Node.js
FROM node:18-bullseye

WORKDIR /app

# Install C++ build dependencies (no OpenCL needed)
RUN apt-get update && apt-get install -y \
    build-essential \
    libglfw3-dev \
    libglew-dev \
    libcglm-dev \
    mesa-common-dev \
    libx11-dev \
    libxrandr-dev \
    libxi-dev \
    libssl-dev \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Copy all source files
COPY . .

# Compile C++ program using Linux build script
RUN chmod +x build-linux.sh
RUN ./build-linux.sh

# Install Node.js dependencies
WORKDIR /app/react-fractal-viewer
RUN npm install

# Build React frontend
RUN npm run build

EXPOSE 3005

CMD ["npm", "run", "start:prod"]

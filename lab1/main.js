const vertexShaderSource = `
attribute vec2 a_coords;
attribute vec3 a_color;
attribute float a_pointsize;

varying vec3 v_color;
varying float v_normalized_center_diameter;

uniform float u_width;
uniform float u_height;
uniform float u_center_diameter;

void main() {
    float x = -1.0 + 2.0*(a_coords.x / u_width);
    float y = 1.0 - 2.0*(a_coords.y / u_height);
    gl_Position = vec4(x, y, 0.0, 1.0);
    v_color = a_color;
    v_normalized_center_diameter = u_center_diameter / a_pointsize;
    gl_PointSize = a_pointsize;
}
`;

const fragmentShaderSource = `
precision mediump float; 

varying vec3 v_color;
varying float v_normalized_center_diameter;

void main() { 
    float distanceFromCenter = distance(gl_PointCoord, vec2(0.5, 0.5)); 
    
    // Drawing the center dot
    if (distanceFromCenter < v_normalized_center_diameter / 2.0) {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    if (distanceFromCenter >= 0.5) { 
        discard; 
    } 

    gl_FragColor = vec4(v_color, 1.0); 
}
`;
/******************************** Begin code ***********************************/

/**************** const variables *************/

const BG_COLOR = [128, 128, 128];
const POINT_DEFAULT_COLOR = [255, 214, 0];

const IS_ANIMATING = false;
const IS_RANDOM_COLORS = true;
const SIZE_CHOICE = -1;
const SHOW_BLACK_DOT = true;

const MAX_DELTA_TIME = 60;

const SPEED_FACTOR = 50;

const CENTER_DOT_DIAMETER = 10;

const INITIAL_POINT_COUNT = 100;

/**************** global variables *************/

let gl;

let POINT_COUNT = INITIAL_POINT_COUNT; // default

let previousTimestamp = 0; // For frame timing
let is_animating = IS_ANIMATING;

let pointCoords;
let pointVelocities;
let pointColors;
let pointSizes;

/**************** buffers *************/

let uniformWidth; // Location of uniform named "u_width"
let uniformHeight; // Location of uniform named "u_height"
let uniformCenterDiameter; // Location of uniform named "u_center_diameter"

let attributePointsize; // Location of attribute named "a_pointsize"
let bufferPointsize; // A vertex buffer object to hold the values for pointsize.

let attributeCoords; // Location of the attribute named "a_coords".
let bufferCoords; // A vertex buffer object to hold the values for coords.

let attributeColor; // Location of the attribute named "a_color".
let bufferColor; // A vertex buffer object to hold the values for color.

/******************************* PROGRAM *****************************/

function createProgram(gl, vertexShaderSource, fragmentShaderSource) {
  let vsh = gl.createShader(gl.VERTEX_SHADER);
  gl.shaderSource(vsh, vertexShaderSource);
  gl.compileShader(vsh);
  if (!gl.getShaderParameter(vsh, gl.COMPILE_STATUS)) {
    throw new Error("Error in vertex shader:  " + gl.getShaderInfoLog(vsh));
  }
  let fsh = gl.createShader(gl.FRAGMENT_SHADER);
  gl.shaderSource(fsh, fragmentShaderSource);
  gl.compileShader(fsh);
  if (!gl.getShaderParameter(fsh, gl.COMPILE_STATUS)) {
    throw new Error("Error in fragment shader:  " + gl.getShaderInfoLog(fsh));
  }
  let prog = gl.createProgram();
  gl.attachShader(prog, vsh);
  gl.attachShader(prog, fsh);
  gl.linkProgram(prog);
  if (!gl.getProgramParameter(prog, gl.LINK_STATUS)) {
    throw new Error("Link error in program:  " + gl.getProgramInfoLog(prog));
  }
  return prog;
}

function recreateBuffers() {
  pointCoords = new Float32Array(2 * POINT_COUNT);
  pointVelocities = new Float32Array(2 * POINT_COUNT);
  pointColors = new Float32Array(3 * POINT_COUNT);
  pointSizes = new Float32Array(1 * POINT_COUNT);
}

function initGL() {
  recreateBuffers();

  let prog = createProgram(gl, vertexShaderSource, fragmentShaderSource);
  gl.useProgram(prog);
  attributeCoords = gl.getAttribLocation(prog, "a_coords");
  bufferCoords = gl.createBuffer();
  attributeColor = gl.getAttribLocation(prog, "a_color");
  bufferColor = gl.createBuffer();
  uniformHeight = gl.getUniformLocation(prog, "u_height");
  uniformWidth = gl.getUniformLocation(prog, "u_width");
  uniformCenterDiameter = gl.getUniformLocation(prog, "u_center_diameter");
  gl.uniform1f(uniformHeight, canvas.height);
  gl.uniform1f(uniformWidth, canvas.width);
  gl.uniform1f(uniformCenterDiameter, CENTER_DOT_DIAMETER);
  attributePointsize = gl.getAttribLocation(prog, "a_pointsize");
  bufferPointsize = gl.createBuffer();
}

function isOverlappingWithCanvas(x, y, size, canvasWidth, canvasHeight) {
  const radius = size / 2;
  let overlap = {
    left: false,
    right: false,
    top: false,
    bottom: false,
    overlapping: false,
  };

  if (x - radius < 0) {
    overlap.left = true;
  }

  if (x + radius > canvasWidth) {
    overlap.right = true;
  }

  if (y - radius < 0) {
    overlap.top = true;
  }

  if (y + radius > canvasHeight) {
    overlap.bottom = true;
  }

  if (overlap.left || overlap.right || overlap.top || overlap.bottom) {
    overlap.overlapping = true;
  }

  return overlap;
}

function getOverlappedPoints(x, y, size, index) {
  const actualCollision = [];
  const radius = size / 2;
  for (let i = 0; i < POINT_COUNT; i++) {
    if (i === index) continue;
    const pointX = pointCoords[2 * i];
    const pointY = pointCoords[2 * i + 1];
    const pointRadius = pointSizes[i] / 2;

    const dx = x - pointX;
    const dy = y - pointY;
    const distance = Math.sqrt(dx * dx + dy * dy);

    if (distance < radius + pointRadius) {
      actualCollision.push({ x: pointX, y: pointY, size: pointSizes[i] });
    }
  }
  return actualCollision;
}

function initPoints(initialPointCoords, initialPointVelocities) {
  for (let i = 0; i < POINT_COUNT; i++) {
    let x = initialPointCoords
      ? initialPointCoords[2 * i]
      : canvas.width * Math.random();
    let y = initialPointCoords
      ? initialPointCoords[2 * i + 1]
      : canvas.height * Math.random();
    let overlapping_canvas = isOverlappingWithCanvas(
      x,
      y,
      pointSizes[i],
      canvas.width,
      canvas.height
    );
    while (
      getOverlappedPoints(x, y, pointSizes[i], i).length > 0 ||
      overlapping_canvas.overlapping
    ) {
      x = canvas.width * Math.random(); // x-coordinate of point
      y = canvas.height * Math.random(); // y-coordinate of point
      overlapping_canvas = isOverlappingWithCanvas(
        x,
        y,
        pointSizes[i],
        canvas.width,
        canvas.height
      );
    }

    pointCoords[2 * i] = x; // x-coordinate of point
    pointCoords[2 * i + 1] = y; // y-coordinate of point
    if (initialPointVelocities) {
      pointVelocities[2 * i] = initialPointVelocities[2 * i];
      pointVelocities[2 * i + 1] = initialPointVelocities[2 * i + 1];
    } else {
      let randomVelocity = 1 + 3 * Math.random();
      let randomAngle = 2 * Math.PI * Math.random();
      pointVelocities[2 * i] = randomVelocity * Math.cos(randomAngle);
      pointVelocities[2 * i + 1] = randomVelocity * Math.sin(randomAngle);
    }
  }
}

function getCollisionVelocity(
  x1,
  y1,
  size1,
  vx1,
  vy1,
  x2,
  y2,
  size2,
  vx2,
  vy2
) {
  // Calculate the unit normal vector
  const dx = x2 - x1;
  const dy = y2 - y1;
  const dist = Math.sqrt(dx * dx + dy * dy);
  const n = { x: dx / dist, y: dy / dist };

  // Calculate the unit tangential vector
  const t = { x: -n.y, y: n.x };

  // Project velocities onto the normal and tangential axes
  const v1n = vx1 * n.x + vy1 * n.y;
  const v1t = vx1 * t.x + vy1 * t.y;
  const v2n = vx2 * n.x + vy2 * n.y;
  const v2t = vx2 * t.x + vy2 * t.y;

  // Compute new normal velocities after the collision
  const m1 = size1;
  const m2 = size2;
  const v1n_prime = (v1n * (m1 - m2) + 2 * m2 * v2n) / (m1 + m2);
  const v2n_prime = (v2n * (m2 - m1) + 2 * m1 * v1n) / (m1 + m2);

  // Combine the tangential and normal velocity components to get the final velocities
  const v1_prime_x = v1n_prime * n.x + v1t * t.x;
  const v1_prime_y = v1n_prime * n.y + v1t * t.y;
  const v2_prime_x = v2n_prime * n.x + v2t * t.x;
  const v2_prime_y = v2n_prime * n.y + v2t * t.y;

  return { v1_prime_x, v1_prime_y, v2_prime_x, v2_prime_y };
}

function updatePositions(deltaTime) {
  for (let i = 0; i < POINT_COUNT; i++) {
    pointCoords[2 * i] += pointVelocities[2 * i] * deltaTime;
    pointCoords[2 * i + 1] += pointVelocities[2 * i + 1] * deltaTime;
  }
}

function handleCollision() {
  let handledCollisions = new Set();

  for (let i = 0; i < POINT_COUNT; i++) {
    const circleA = {
      x: pointCoords[2 * i],
      y: pointCoords[2 * i + 1],
      size: pointSizes[i],
    };

    const potentialCollisions = getOverlappedPoints(
      circleA.x,
      circleA.y,
      circleA.size,
      i
    );

    for (const other of potentialCollisions) {
      const j = pointCoords.indexOf(other.x) / 2; // Assuming each x,y is paired

      // Generate a unique key for the collision pair (smallest index first)
      const collisionKey = i < j ? `${i}-${j}` : `${j}-${i}`;

      // Skip if this collision has already been handled
      if (handledCollisions.has(collisionKey)) continue;

      // Get the velocities for the collision
      const velocities = getCollisionVelocity(
        circleA.x,
        circleA.y,
        circleA.size,
        pointVelocities[2 * i],
        pointVelocities[2 * i + 1],
        other.x,
        other.y,
        other.size,
        pointVelocities[2 * j],
        pointVelocities[2 * j + 1]
      );

      // Overlap correction
      // const dx = other.x - circleA.x;
      // const dy = other.y - circleA.y;
      // const distance = Math.sqrt(dx * dx + dy * dy) || 0.001;
      // const overlap = (circleA.size + other.size) / 2 - distance;
      // const correctionX = (dx / distance) * overlap;
      // const correctionY = (dy / distance) * overlap;

      // circleA.x -= correctionX * 0.5;
      // circleA.y -= correctionY * 0.5;
      // other.x += correctionX * 0.5;
      // other.y += correctionY * 0.5; // fix exact overlapp correction

      // Overlap correction
      const dx = other.x - circleA.x;
      const dy = other.y - circleA.y;
      const distance = Math.sqrt(dx * dx + dy * dy);
      const radiusSum = (circleA.size / 2) + (other.size / 2);

      if (distance < radiusSum) {
          // Calculate the penetration depth
          const p = radiusSum - distance;
          
          // Normalize the distance vector
          const nx = dx / distance;
          const ny = dy / distance;

          // Correct positions based on penetration depth
          const correctionX = nx * p;
          const correctionY = ny * p;

          circleA.x -= correctionX * 0.5;
          circleA.y -= correctionY * 0.5;
          other.x += correctionX * 0.5;
          other.y += correctionY * 0.5;
      }

      // Update velocities
      pointVelocities[2 * i] = velocities.v1_prime_x;
      pointVelocities[2 * i + 1] = velocities.v1_prime_y;
      pointVelocities[2 * j] = velocities.v2_prime_x;
      pointVelocities[2 * j + 1] = velocities.v2_prime_y;

      // Update positions to the corrected positions
      pointCoords[2 * j] = other.x;
      pointCoords[2 * j + 1] = other.y;

      // Mark this collision as handled
      handledCollisions.add(collisionKey);
    }

    // Canvas boundary check for circleA
    let canvas_overlap = isOverlappingWithCanvas(
      circleA.x,
      circleA.y,
      circleA.size,
      canvas.width,
      canvas.height
    );
    if (canvas_overlap.overlapping) {
      if (canvas_overlap.left) {
        circleA.x = circleA.size / 2;
        pointVelocities[2 * i] *= -1;
      }
      if (canvas_overlap.right) {
        circleA.x = canvas.width - circleA.size / 2;
        pointVelocities[2 * i] *= -1;
      }
      if (canvas_overlap.top) {
        circleA.y = circleA.size / 2;
        pointVelocities[2 * i + 1] *= -1;
      }
      if (canvas_overlap.bottom) {
        circleA.y = canvas.height - circleA.size / 2;
        pointVelocities[2 * i + 1] *= -1;
      }
    }

    // Update positions to the corrected positions
    pointCoords[2 * i] = circleA.x;
    pointCoords[2 * i + 1] = circleA.y;
  }
}

function handleFrame(timestamp) {
  if (!previousTimestamp) {
    previousTimestamp = timestamp;
    requestAnimationFrame(handleFrame);
    return;
  }

  let deltaTime = (timestamp - previousTimestamp) / SPEED_FACTOR;

  if (deltaTime > MAX_DELTA_TIME) {
    deltaTime = 16.67 / SPEED_FACTOR; // A safe value to prevent huge jumps
  }

  if (is_animating) {
    updatePositions(deltaTime);
    handleCollision();
  }

  draw();

  previousTimestamp = timestamp;

  if (is_animating) {
    requestAnimationFrame(handleFrame);
  }
}

function draw() {
  gl.clearColor(
    (1 / 255) * BG_COLOR[0],
    (1 / 255) * BG_COLOR[1],
    (1 / 255) * BG_COLOR[2],
    1
  );
  gl.clear(gl.COLOR_BUFFER_BIT);

  // Bind the point size buffer
  gl.bindBuffer(gl.ARRAY_BUFFER, bufferPointsize);
  gl.bufferData(gl.ARRAY_BUFFER, pointSizes, gl.STATIC_DRAW);
  gl.enableVertexAttribArray(attributePointsize);
  gl.vertexAttribPointer(attributePointsize, 1, gl.FLOAT, false, 0, 0);

  // Bind the point coordinate buffer
  gl.bindBuffer(gl.ARRAY_BUFFER, bufferCoords);
  gl.bufferData(gl.ARRAY_BUFFER, pointCoords, gl.STATIC_DRAW);
  gl.enableVertexAttribArray(attributeCoords);
  gl.vertexAttribPointer(attributeCoords, 2, gl.FLOAT, false, 0, 0);

  // Bind the point color buffer
  gl.bindBuffer(gl.ARRAY_BUFFER, bufferColor);
  gl.bufferData(gl.ARRAY_BUFFER, pointColors, gl.STATIC_DRAW);
  gl.enableVertexAttribArray(attributeColor);
  gl.vertexAttribPointer(attributeColor, 3, gl.FLOAT, false, 0, 0);

  gl.drawArrays(gl.POINTS, 0, POINT_COUNT);
}

function handleColorCheckbox() {
  let value = document.getElementById("colorCheckbox").checked;
  for (let i = 0; i < POINT_COUNT; i++) {
    if (value) {
      pointColors[3 * i] = Math.random();
      pointColors[3 * i + 1] = Math.random();
      pointColors[3 * i + 2] = Math.random();
    } else {
      pointColors[3 * i] = (1 / 255) * POINT_DEFAULT_COLOR[0];
      pointColors[3 * i + 1] = (1 / 255) * POINT_DEFAULT_COLOR[1];
      pointColors[3 * i + 2] = (1 / 255) * POINT_DEFAULT_COLOR[2];
    }
  }
  draw();
}

function handleSizeChoice() {
  let value = Number(document.getElementById("sizeChoice").value);
  for (let i = 0; i < POINT_COUNT; i++) {
    pointSizes[i] =
      value !== -1 ? value : CENTER_DOT_DIAMETER + 50 * Math.random();
  }
  draw();
}

function handleAnimationCheckbox() {
  let value = document.getElementById("animateCheckbox").checked;
  is_animating = value;
  if (value) {
    requestAnimationFrame(handleFrame);
  }
  draw();
}

function handleBlackDotCheckbox() {
  let value = document.getElementById("blackDotVisible").checked;
  if (value) {
    gl.uniform1f(uniformCenterDiameter, CENTER_DOT_DIAMETER);
  } else {
    gl.uniform1f(uniformCenterDiameter, 0);
  }
  draw();
}

function handleSimChoice() {
  let value = Number(document.getElementById("simChoice").value);
  let defaultCoords;
  let defaultVelocities;
  console.log(value);
  switch (value) {
    case 1:
      POINT_COUNT = INITIAL_POINT_COUNT;
      document.getElementById("sizeChoice").value = "-1";
      break;
    case 2:
      POINT_COUNT = 2;
      // both move towards eachother horizontally
      defaultCoords = new Float32Array([
        canvas.width / 4,
        canvas.height / 2,
        (3 * canvas.width) / 4,
        canvas.height / 2,
      ]);
      defaultVelocities = new Float32Array([3, 0, -3, 0]);
      document.getElementById("sizeChoice").value = "32";
      break;
    case 3:
      POINT_COUNT = 2;
      // One moves horizontally, the other is still
      defaultCoords = new Float32Array([
        canvas.width / 4,
        canvas.height / 2,
        (3 * canvas.width) / 4,
        canvas.height / 2,
      ]);
      defaultVelocities = new Float32Array([3, 0, 0, 0]);
      document.getElementById("sizeChoice").value = "32";
      break;
    case 4:
      POINT_COUNT = 2;
      // Both move towards eachother diagonally
      defaultCoords = new Float32Array([
        canvas.width / 4,
        canvas.height / 2,
        (3 * canvas.width) / 4,
        canvas.height / 2,
      ]);
      defaultVelocities = new Float32Array([3, 0.5, -3, 0.5]);
      document.getElementById("sizeChoice").value = "32";
      break;
    default:
      POINT_COUNT = INITIAL_POINT_COUNT;
      document.getElementById("sizeChoice").value = "-1";
  }

  if (POINT_COUNT !== pointCoords.length / 2) {
    recreateBuffers();
    handleAnimationCheckbox();
    handleColorCheckbox();
    handleBlackDotCheckbox();
  }

  handleSizeChoice();

  initPoints(defaultCoords, defaultVelocities);

  requestAnimationFrame(handleFrame);
}

function init() {
  try {
    canvas = document.getElementById("webglcanvas");
    let options = {
      // no need for alpha channel or depth buffer in this program
      alpha: false,
      depth: false,
    };
    gl = canvas.getContext("webgl2", options);
    if (!gl) {
      throw "Browser does not support WebGL";
    }
  } catch (e) {
    document.getElementById("canvas-holder").innerHTML =
      "<p>Sorry, could not get a WebGL graphics context.</p>";
    return;
  }

  try {
    initGL(); // initialize the WebGL graphics context
  } catch (e) {
    document.getElementById("canvas-holder").innerHTML =
      "<p>Sorry, could not initialize the WebGL graphics context: " +
      e +
      "</p>";
    return;
  }

  document.getElementById("animateCheckbox").onchange = handleAnimationCheckbox;
  document.getElementById("colorCheckbox").onchange = handleColorCheckbox;
  document.getElementById("sizeChoice").onchange = handleSizeChoice;
  document.getElementById("blackDotVisible").onchange = handleBlackDotCheckbox;
  document.getElementById("simChoice").onchange = handleSimChoice;

  document.getElementById("animateCheckbox").checked = IS_ANIMATING;
  document.getElementById("colorCheckbox").checked = IS_RANDOM_COLORS;
  document.getElementById("sizeChoice").value = SIZE_CHOICE;
  document.getElementById("blackDotVisible").checked = SHOW_BLACK_DOT;
  document.getElementById("simChoice").value = "1";

  handleAnimationCheckbox();
  handleColorCheckbox();
  handleBlackDotCheckbox();

  handleSimChoice();
}

window.onload = init; // Arrange for init() to be called after page has loaded.

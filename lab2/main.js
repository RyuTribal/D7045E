const vertexShaderSource = `
attribute vec2 a_coords;

uniform vec3 u_color;
uniform vec3 u_edge_color;

varying vec3 v_color;

void main() {
    vec3 v_color = u_color;

    gl_Position = vec4(a_coords, 1.0, 1.0);

    gl_PointSize = 5.0;
}
`;

const fragmentShaderSource = `
precision mediump float;

varying vec3 v_color;

void main() { 
    gl_FragColor = vec4(v_color, 1.0);
}
`;
/******************************** Begin code ***********************************/

/**************** const variables *************/

const BG_COLOR = [255, 255, 255];
const DEFAULT_COLOR = [128, 128, 128];
const EDGE_DEFAULT_COLOR = [0, 0, 0];

const INITIAL_POINT_COUNT = 10;

const MAX_DELTA_TIME = 60;

const SPEED_FACTOR = 50;

/**************** global variables *************/

let gl;

let POINT_COUNT = INITIAL_POINT_COUNT; // default

let point_color = DEFAULT_COLOR;

let previousTimestamp = 0; // For frame timing

let triangleCoords;

let pointCoords = [];

let shouldDrawTriangles = false;

/**************** buffers *************/

let uniformColor; // Location of uniform named "u_color"
let uniformEdgeColor; // Location of uniform named "u_edge_color"

let attributeCoords; // Location of the attribute named "a_coords".
let bufferCoords; // A vertex buffer object to hold the values for coords.

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
  triangleCoords = new Float32Array(2 * POINT_COUNT);
}

function generatePoints() {
  for (let i = 0; i < POINT_COUNT; i++) {
    pointCoords[i] = [Math.random() * 2 - 1, Math.random() * 2 - 1];
    triangleCoords[2 * i] = pointCoords[i][0];
    triangleCoords[2 * i + 1] = pointCoords[i][1];
  }
}

function sortPoints() {
    
}

function setupTriangles() {
    sortPoints();
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

  draw();

  previousTimestamp = timestamp;

  requestAnimationFrame(handleFrame);
}

function draw() {
  gl.clearColor(
    (1 / 255) * BG_COLOR[0],
    (1 / 255) * BG_COLOR[1],
    (1 / 255) * BG_COLOR[2],
    1
  );
  gl.clear(gl.COLOR_BUFFER_BIT);

  gl.bindBuffer(gl.ARRAY_BUFFER, bufferCoords);
  gl.bufferData(gl.ARRAY_BUFFER, triangleCoords, gl.STATIC_DRAW);
  gl.enableVertexAttribArray(attributeCoords);
  gl.vertexAttribPointer(attributeCoords, 2, gl.FLOAT, false, 0, 0);

  if (shouldDrawTriangles) {
    gl.drawArrays(gl.TRIANGLES, 0, POINT_COUNT);
    gl.drawArrays(gl.LINE_LOOP, 0, POINT_COUNT);
  }

  gl.uniform3f(
    uniformEdgeColor,
    EDGE_DEFAULT_COLOR[0] / 255,
    EDGE_DEFAULT_COLOR[1] / 255,
    EDGE_DEFAULT_COLOR[2] / 255
  );

  gl.drawArrays(gl.POINTS, 0, POINT_COUNT);
}
function handleSetChoice() {
  shouldDrawTriangles = false;
  document.getElementById("shouldDraw").checked = false;
  POINT_COUNT = document.getElementById("setChoice").value;
  document.getElementById("pointCount").value = POINT_COUNT;
  recreateBuffers();

  generatePoints();
}

function handlePointCount(pointCount) {
  shouldDrawTriangles = false;
  document.getElementById("shouldDraw").checked = false;
  if (pointCount) {
    if (pointCount < 3) {
      POINT_COUNT = 3;
    } else {
      POINT_COUNT = pointCount;
    }
  } else {
    POINT_COUNT = document.getElementById("pointCount").value;
  }
  recreateBuffers();

  generatePoints();
}

function initGL() {
  recreateBuffers();

  let prog = createProgram(gl, vertexShaderSource, fragmentShaderSource);
  gl.useProgram(prog);
  attributeCoords = gl.getAttribLocation(prog, "a_coords");
  bufferCoords = gl.createBuffer();
  uniformColor = gl.getUniformLocation(prog, "u_color");
  uniformEdgeColor = gl.getUniformLocation(prog, "u_edge_color");
  gl.uniform3fv(uniformColor, DEFAULT_COLOR);
  gl.uniform3fv(uniformEdgeColor, EDGE_DEFAULT_COLOR);
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

  document.getElementById("setChoice").onchange = handleSetChoice;
  document.getElementById("pointCount").onchange = handlePointCount;
  document.getElementById("shouldDraw").onchange = function () {
    shouldDrawTriangles = document.getElementById("shouldDraw").checked;
    if (shouldDrawTriangles) {
      setupTriangles();
    }
  };

  document.getElementById("setChoice").value = "10";
  document.getElementById("pointCount").value = 10;
  document.getElementById("shouldDraw").checked = false;

  handleSetChoice();
  handlePointCount();

  requestAnimationFrame(handleFrame);
}

window.onload = init;

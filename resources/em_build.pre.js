/**
 * @fileOverview javascript code inserted before emscripten compiler output.
 * Includes
 */
var Module = {
  noInitialRun: true,
  arguments: [],
  preInit() {},
  onRuntimeInitialized(){}
};
/**
 * half-baked polyfill for Shared*Array types,
 * which are not included in the final Atomics
 * standard (or any implementation)
 * 
 * For now, the emscripten pthreads port does not
 * work great——any multiprocessing should be done with
 * web worker api in emscripten.h
 *
/* window.SharedInt8Array = Int8Array;
 * window.SharedInt16Array = Int16Array;
 * window.SharedInt32Array = Int32Array;
 * 
 * window.SharedUint8Array = Uint8Array;
 * window.SharedUint16Array = Uint16Array;
 * window.SharedUint32Array = Uint32Array;
 * 
 * window.SharedFloat32Array = Float32Array;
 * window.SharedFloat64Array = Float64Array; */

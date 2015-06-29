attribute vec3 position;

void main()
{
  gl_Position = ftransform();
}
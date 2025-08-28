
# Fractal Drawer 2.0

This is an application letting users create various fractals by drawing a few lines. Compared to version 1, it includes:
- better project structure (separate modules)
- CPU parallelism (instead of many calls to GPU) - not exactly an improvement on its own, but this lets choosing different colors for lines of different sizes
- configuration file (in res)
- optional huge zoom levels thanks to custom double type with 1024 bit mantissa (Zoom in for a few minutes even at max speed! Though it's slow when there are lots of lines)
- per-fractal deletion.
## Default controls
Click - draw root line; shift + click - draw branch line; Enter - render the fractal; Z, X - zoom; Esc - zoom reset; Backspace - delete last fractal.
# Infinite Stamp
A branch with another module for drawing not fractals, but shapes (as in Paint) and screens in screens in screens.
# License
Use this project freely! I won't license it.
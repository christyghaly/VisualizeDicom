Render a slice of DICOM SCAN saved (jpg file) in a QtOpenGLWidget Control.
In this project, the used type of Widgets are QTOpenGlWidget and QWidget with a file dialog to choose the image to be rendered.
OpenGL commands are used to clear buffers, allowing changes made to the images in the UI to be rendered more quickly.
QMenu is used to choose between Zoom In, Zoom Out, and options to Increase or Decrease brightness and contrast. Additionally, the user has the ability to move the image inside the OpenGL widget.
## Used Qt Related Techniques:
- **QtOpenGLWidget**
- **QWidget**
- **QPainter**
- **QImage**
- **QFileDialog**
- **QToolbar**
- **QColor**
- **QPixel**

## Used OpenGL Related Commands:
- OpenGL commands related to initializing (e.g., `glClear`, `glLoadIdentity`, `glViewport`, ...).

For more details please check the attached pdf file [DicomVisualization.pdf](DicomVisualization.pdf) and check the following link for a [quick demo](https://www.youtube.com/watch?v=zQMFfF6kHXk)

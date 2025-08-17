#include <utility>
#include "src/Configuration/Configuration.h"
#include "src/Camera/BasicCamera/BasicCamera.h"
#include "src/DataStructure2D/BasicDataStructure2D/BasicDataStructure2D.h"
#include "src/LongDouble/BasicLongDouble/BasicLongDouble.h"
#include "src/RecursiveRenderer/BasicRecursiveRenderer/BasicRecursiveRenderer.h"
#include "src/Shapes/Rectangle.h"
#include "src/helpers/Position.h"

int main(int argc, char const *argv[])
{
  Configuration configuration(argc, argv);

  BasicDataStructure2D<BasicLongDouble>* p_data_structure = new BasicDataStructure2D<BasicLongDouble>;

  BasicCamera<BasicLongDouble>* p_camera = new BasicCamera<BasicLongDouble>(
    std::make_pair(
      Position<BasicLongDouble>{BasicLongDouble(0), BasicLongDouble(0)},
      Position<BasicLongDouble>{BasicLongDouble(1), BasicLongDouble(1)}
    )
  );

  BasicRecursiveRenderer<BasicLongDouble> recursiveRenderer(p_data_structure, p_camera, configuration);

  recursiveRenderer.zoom_in(Position<BasicLongDouble>{BasicLongDouble(0.5), BasicLongDouble(0.5)}, 0.5);

  return 0;
}

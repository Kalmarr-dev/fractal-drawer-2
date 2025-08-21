#include "Fractal.h"

#include "../../../helpers/ScaleRotationMatrix.h"
#include <queue>
#include <iostream>

template<typename T>
Fractal<T>::Fractal(FractalPart<T>* root_fractal_part) : root_fractal_part(root_fractal_part) {
  // all_fractal_parts.push_back(root_fractal_part);
  peripheral_fractal_parts.insert(root_fractal_part);
}

template<typename T>
std::vector<FractalPart<T>*> Fractal<T>::update_on_zoom(ICamera<T>* p_camera, int MAXLINES, T MIN_LINE_SIZE, int MAX_DEPTH) {
  std::vector<FractalPart<T>*> new_fractal_parts;

  auto camera_corners = p_camera->get_camera_corners();

  std::vector<FractalPart<T>*> fractal_parts_to_process;
  fractal_parts_to_process.reserve(this->peripheral_fractal_parts.size());
  for (auto &&i : this->peripheral_fractal_parts)
  {
    fractal_parts_to_process.push_back(i);
  }

  int new_lines_count = 0;

  #pragma omp parallel for
  for (int i = 0; i < fractal_parts_to_process.size(); i++)
  {
    // #pragma omp critical(new_lines_count)
    if (new_lines_count > MAXLINES)
    {
      continue;
    }
    auto& current_fractal_part = fractal_parts_to_process[i];
    auto root_line = current_fractal_part->get_root_line();
    auto corners = current_fractal_part->get_corners();
    auto direction_lines = current_fractal_part->get_direction_lines();
    #pragma omp parallel for
    for (auto &reflection_line : current_fractal_part->get_last_reflection_lines())
    {
      if (current_fractal_part->find_used_last_reflection_line(reflection_line))
      {
        continue;
      }
      if (new_lines_count > MAXLINES)
      {
        continue;
      }
      
      // find bounding coordinates (min_x, etc) of potential new FractalPart
      ScaleRotationMatrix<T> RTRVectorsScaleRotationMatrix(root_line, *reflection_line);
      Line<T> RSTRSVector = Line<T>(Position<T>(root_line.a.x, root_line.a.y), Position<T>(reflection_line->a.x, reflection_line->a.y));
      ScaleRotationMatrix<T> RTRSTRSVectorsScaleRotationMatrix(root_line, RSTRSVector);

      Line<T> RSTXYVector1 = Line<T>(Position<T>(0, 0), Position<T>(corners.first.x - root_line.a.x, corners.first.y - root_line.a.y));
      Line<T> RSTXYVector2 = Line<T>(Position<T>(0, 0), Position<T>(corners.first.x - root_line.a.x, corners.second.y - root_line.a.y));
      Line<T> RSTXYVector3 = Line<T>(Position<T>(0, 0), Position<T>(corners.second.x - root_line.a.x, corners.first.y - root_line.a.y));
      Line<T> RSTXYVector4 = Line<T>(Position<T>(0, 0), Position<T>(corners.second.x - root_line.a.x, corners.second.y - root_line.a.y));
      
      Line<T> transformedX1 = RTRVectorsScaleRotationMatrix.MultiplyByVector(RSTXYVector1);
      Line<T> transformedX2 = RTRVectorsScaleRotationMatrix.MultiplyByVector(RSTXYVector2);
      Line<T> transformedX3 = RTRVectorsScaleRotationMatrix.MultiplyByVector(RSTXYVector3);
      Line<T> transformedX4 = RTRVectorsScaleRotationMatrix.MultiplyByVector(RSTXYVector4);

      // second coordinates are the transformed {min_x, miny}, {min_x, max_y} etc
      transformedX1 = Line<T>(Position<T>(reflection_line->a.x, reflection_line->a.y), Position<T>(reflection_line->a.x + transformedX1.b.x, reflection_line->a.y + transformedX1.b.y));
      transformedX2 = Line<T>(Position<T>(reflection_line->a.x, reflection_line->a.y), Position<T>(reflection_line->a.x + transformedX2.b.x, reflection_line->a.y + transformedX2.b.y));
      transformedX3 = Line<T>(Position<T>(reflection_line->a.x, reflection_line->a.y), Position<T>(reflection_line->a.x + transformedX3.b.x, reflection_line->a.y + transformedX3.b.y));
      transformedX4 = Line<T>(Position<T>(reflection_line->a.x, reflection_line->a.y), Position<T>(reflection_line->a.x + transformedX4.b.x, reflection_line->a.y + transformedX4.b.y));

      T min_x = std::min(std::min(transformedX1.b.x, transformedX2.b.x), std::min(transformedX3.b.x, transformedX4.b.x));
      T max_x = std::max(std::max(transformedX1.b.x, transformedX2.b.x), std::max(transformedX3.b.x, transformedX4.b.x));
      T min_y = std::min(std::min(transformedX1.b.y, transformedX2.b.y), std::min(transformedX3.b.y, transformedX4.b.y));
      T max_y = std::max(std::max(transformedX1.b.y, transformedX2.b.y), std::max(transformedX3.b.y, transformedX4.b.y));

      std::pair<Position<T>, Position<T>> new_sorted_corners(Position<T>(min_x, min_y), Position<T>(max_x, max_y));

      if (FractalPart<T>::intersects_with(new_sorted_corners, camera_corners) && MIN_LINE_SIZE < max_x + max_y - min_x - min_y)
      {
        std::list<Line<T>> new_direction_lines;
        for (auto &&line : direction_lines)
        {
          Line<T> RSTLSVector = Line<T>(Position<T>(root_line.a.x, root_line.a.y), Position<T>(line.a.x, line.a.y));
          ScaleRotationMatrix<T> RSTLSVectorsScaleRotationMatrix(root_line, RSTLSVector);
          Line<T> start_to_start_vector = RSTLSVectorsScaleRotationMatrix.MultiplyByVector(*reflection_line);
          Line<T> new_direction_line_vector = RTRVectorsScaleRotationMatrix.MultiplyByVector(line);
          T x0, y0, x1, y1;
          x0 = reflection_line->a.x + start_to_start_vector.b.x;
          y0 = reflection_line->a.y + start_to_start_vector.b.y;
          x1 = x0 + new_direction_line_vector.b.x;
          y1 = y0 + new_direction_line_vector.b.y;
          new_direction_lines.push_back(Line<T>(Position<T>(x0, y0), Position<T>(x1, y1)));
        }

        FractalStub<T>* p_fractal_stub = new FractalStub<T>(*reflection_line, new_direction_lines);
        FractalPart<T>* p_new_fractal_part = new FractalPart<T>(p_fractal_stub, MAXLINES, MIN_LINE_SIZE, MAX_DEPTH);
        
        #pragma omp critical(new_fractal_parts)
        {
          new_fractal_parts.push_back(p_new_fractal_part);
        }
        // #pragma omp critical(all_fractal_parts)
        // {
        //   all_fractal_parts.push_back(p_new_fractal_part);
        // }
        #pragma omp critical(peripheral_fractal_parts)
        {
          peripheral_fractal_parts.insert(p_new_fractal_part);
        }
        #pragma omp critical(new_lines_count)
        {
          new_lines_count += p_new_fractal_part->get_lines().size();
        }

        current_fractal_part->insert_used_last_reflection_line(reflection_line);
        //   if (current_fractal_part->get_used_last_reflection_lines_size() >= current_fractal_part->get_last_reflection_lines().size())
        //   {
          // #pragma omp critical(peripheral_fractal_parts)
          // {
        //     peripheral_fractal_parts.erase(current_fractal_part);
        // }
        //   }
        
        // auto new_corners_check = p_new_fractal_part->get_corners();
        // auto offset = new T(0);
        // std::cout << new_sorted_corners.first.x.get_double(offset, 1) << " " << new_corners_check.first.x.get_double(offset, 1) << '\n';
        // std::cout << new_sorted_corners.first.y.get_double(offset, 1) << " " << new_corners_check.first.y.get_double(offset, 1) << '\n';
        // std::cout << new_sorted_corners.second.x.get_double(offset, 1) << " " << new_corners_check.second.x.get_double(offset, 1) << '\n';
        // std::cout << new_sorted_corners.second.y.get_double(offset, 1) << " " << new_corners_check.second.y.get_double(offset, 1) << '\n';
        // std::cout << '\n';
      }
    }
  }

  // std::cout << "this->all_fractal_parts.size() " << this->all_fractal_parts.size() << '\n';
  return new_fractal_parts;
}
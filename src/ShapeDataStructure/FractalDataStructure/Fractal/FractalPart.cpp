#include "FractalPart.h"

#include "../../../helpers/ScaleRotationMatrix.h"

template<typename T>
FractalPart<T>::FractalPart(FractalStub<T>* p_fractal_stub, int MAXLINES, T MIN_LINE_SIZE, int MAX_DEPTH) {
  this->root_line = p_fractal_stub->get_root_line();
  for (auto &&i : p_fractal_stub->get_direction_lines())
  {
    this->direction_lines.push_back(i);
  }
  
  ScaleRotationMatrix<T>* MTDVectorsScaleRotationMatrices[this->direction_lines.size()]; //main-to-direction
  ScaleRotationMatrix<T>* MTMTDVectorsScaleRotationMatrices[this->direction_lines.size()]; //main-to-main-start-to-direction-start

  for (size_t i = 0; i < this->direction_lines.size(); i++) {
    MTDVectorsScaleRotationMatrices[i] = new ScaleRotationMatrix<T>(this->root_line, this->direction_lines[i]);
  }

  for (size_t i = 0; i < this->direction_lines.size(); i++) {
    Line<T> MTDLine = Line<T>(Position<T>(this->root_line.a.x, this->root_line.a.y), Position<T>(this->direction_lines[i].a.x, this->direction_lines[i].a.y));
    MTMTDVectorsScaleRotationMatrices[i] = new ScaleRotationMatrix<T>(this->root_line, MTDLine);
  }


  std::vector<Line<T>> reflectionLines;
  reflectionLines.reserve(MAXLINES + 100);
  reflectionLines.push_back(this->root_line);
  this->linesOnTheLayer.push_back(1);
  for (size_t i = 1; i < this->direction_lines.size() + 1; i++) {
    reflectionLines.push_back(this->direction_lines[i - 1]);
  }
  this->linesOnTheLayer.push_back(this->direction_lines.size());

  int reflectionLinesOnPrevLayer = this->direction_lines.size();
  int reflectionLinesOnPrevLayers = 1 + reflectionLinesOnPrevLayer;
  int reflectionLinesOnThisLayer = 0;

  std::vector<int> lastReflectionLinesIndices;
  lastReflectionLinesIndices.reserve(1000);

  bool onThisLayerLinesWereCreated = true;
  int depth = 1;
  while (reflectionLinesOnPrevLayers < MAXLINES && onThisLayerLinesWereCreated && depth < MAX_DEPTH) { // TODO lastReflectionLinesIndices won't be full if this is true
    onThisLayerLinesWereCreated = false;
    depth++;

    for (int i = reflectionLinesOnPrevLayers - reflectionLinesOnPrevLayer; i < reflectionLinesOnPrevLayers; i++) {
      if (reflectionLines[i].get_linear_size_squared() < MIN_LINE_SIZE * MIN_LINE_SIZE) { // TODO MIN_LINE_SIZE**2 may be small
        lastReflectionLinesIndices.push_back(i);
        continue;
      }
      for (size_t dLType = 0; dLType < this->direction_lines.size(); dLType++) {
        Line<T> startToStartVector = MTMTDVectorsScaleRotationMatrices[dLType]->MultiplyByVector(reflectionLines[i]);
        Line<T> reflectionLineVector = MTDVectorsScaleRotationMatrices[dLType]->MultiplyByVector(reflectionLines[i]);
        T x0, y0, x1, y1;
        x0 = reflectionLines[i].a.x + startToStartVector.b.x;
        y0 = reflectionLines[i].a.y + startToStartVector.b.y;
        x1 = x0 + reflectionLineVector.b.x;
        y1 = y0 + reflectionLineVector.b.y;
        Line<T> reflectionLine = Line<T>(Position<T>(x0, y0), Position<T>(x1, y1));
        reflectionLines.push_back(reflectionLine);
        reflectionLinesOnThisLayer++;
        onThisLayerLinesWereCreated = true;
      }
    }

    this->linesOnTheLayer.push_back(reflectionLinesOnThisLayer);

    reflectionLinesOnPrevLayers += reflectionLinesOnThisLayer;
    reflectionLinesOnPrevLayer = reflectionLinesOnThisLayer;
    reflectionLinesOnThisLayer = 0;
  }


  for (int i = reflectionLinesOnPrevLayers - reflectionLinesOnPrevLayer; i < reflectionLinesOnPrevLayers; i++) {
    lastReflectionLinesIndices.push_back(i);
  }

  this->lastReflectionLines = std::vector<Line<T>*>(lastReflectionLinesIndices.size());
  for (size_t i = 0; i < lastReflectionLinesIndices.size(); i++) {
    auto& reflection_line = reflectionLines[lastReflectionLinesIndices[i]];
    this->lastReflectionLines[i] = new Line<T>({reflection_line.a.x, reflection_line.a.y}, {reflection_line.b.x, reflection_line.b.y});
  }

  // std::cout << "Last reflection lines count: " << lastReflectionLinesCount << '\n';

  this->lines = reflectionLines;

  // destructors
  for (size_t i = 0; i < this->direction_lines.size(); i++) {
    delete MTDVectorsScaleRotationMatrices[i];
  }
  for (size_t i = 0; i < this->direction_lines.size(); i++) {
    delete MTMTDVectorsScaleRotationMatrices[i];
  }

  this->min_x = std::min(this->lines[0].a.x, this->lines[0].b.x);
  this->max_x = std::max(this->lines[0].a.x, this->lines[0].b.x);
  this->min_y = std::min(this->lines[0].a.y, this->lines[0].b.y);
  this->max_y = std::max(this->lines[0].a.y, this->lines[0].b.y);
  for (auto &&i : this->lines)
  {
    this->min_x = std::min(i.a.x, std::min(i.b.x, this->min_x));
    this->max_x = std::max(i.a.x, std::max(i.b.x, this->max_x));
    this->min_y = std::min(i.a.y, std::min(i.b.y, this->min_y));
    this->max_y = std::max(i.a.y, std::max(i.b.y, this->max_y));
  }
  // TODO could add them, but this would render unnesessary fractalparts
  this->linear_size = std::max(this->max_x - this->min_x, this->max_y - this->min_y);
  
}

template<typename T>
std::vector<Line<T>> FractalPart<T>::get_lines() {
  return this->lines;
}

template<typename T>
T FractalPart<T>::get_size() {
  return this->linear_size;
}

template<typename T>
std::pair<Position<T>, Position<T>> FractalPart<T>::get_corners() {
  return std::make_pair(Position<T>(this->min_x, this->min_y), Position<T>(this->max_x, this->max_y));
}


template<typename T>
bool FractalPart<T>::intersects_with(std::pair<Position<T>, Position<T>> sorted_corners, std::pair<Position<T>, Position<T>> corners) {
  T min_x = sorted_corners.first.x;
  T max_x = sorted_corners.second.x;
  T min_y = sorted_corners.first.y;
  T max_y = sorted_corners.second.y;
  if (
    max_x < std::min(corners.first.x, corners.second.x)
    || std::max(corners.first.x, corners.second.x) < min_x
    || max_y < std::min(corners.first.y, corners.second.y)
    || std::max(corners.first.y, corners.second.y) < min_y
  ) {
    return false;
  } else {
    return true;
  }
}

template<typename T>
Line<T> FractalPart<T>::get_root_line() {
  return this->root_line;
}

template<typename T>
std::vector<Line<T>> FractalPart<T>::get_direction_lines() {
  return this->direction_lines;
}

template<typename T>
std::vector<Line<T>*> FractalPart<T>::get_last_reflection_lines() {
  return this->lastReflectionLines;
}

template<typename T>
void FractalPart<T>::insert_used_last_reflection_line(Line<T>* p_line) {
  this->used_last_reflection_lines.insert(p_line);
}

template<typename T>
bool FractalPart<T>::find_used_last_reflection_line(Line<T>* p_line) {
  return this->used_last_reflection_lines.count(p_line) > 0;
}

template<typename T>
int FractalPart<T>::get_used_last_reflection_lines_size() {
  return this->used_last_reflection_lines.size();
}

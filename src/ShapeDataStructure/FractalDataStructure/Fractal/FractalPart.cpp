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

  for (int i = 0; i < this->direction_lines.size(); i++) {
    MTDVectorsScaleRotationMatrices[i] = new ScaleRotationMatrix<T>(this->root_line, this->direction_lines[i]);
  }

  for (int i = 0; i < this->direction_lines.size(); i++) {
    Line<T> MTDLine = Line<T>(Position<T>(this->root_line.a.x, this->root_line.a.y), Position<T>(this->direction_lines[i].a.x, this->direction_lines[i].a.y));
    MTMTDVectorsScaleRotationMatrices[i] = new ScaleRotationMatrix<T>(this->root_line, MTDLine);
  }


  std::vector<Line<T>> reflectionLines;
  reflectionLines.reserve(MAXLINES + 100); // TODO check if [0] is possible after reserve
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
  while (reflectionLinesOnPrevLayers < MAXLINES && onThisLayerLinesWereCreated && depth < MAX_DEPTH) {
    onThisLayerLinesWereCreated = false;
    depth++;

    for (int i = reflectionLinesOnPrevLayers - reflectionLinesOnPrevLayer; i < reflectionLinesOnPrevLayers; i++) {
      if (reflectionLines[i].get_linear_size_squared() < MIN_LINE_SIZE * MIN_LINE_SIZE) { // TODO MIN_LINE_SIZE**2 may be small
        lastReflectionLinesIndices.push_back(i);
        continue;
      }
      for (int dLType = 0; dLType < this->direction_lines.size(); dLType++) {
        int currentIndex = reflectionLinesOnPrevLayers + reflectionLinesOnThisLayer;
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

  int reflectionLinesCount = reflectionLinesOnPrevLayers;

  // was needed when "reflectionLines = new Line[10000000]""
  // std::vector<Line<T>> _reflectionLines(reflectionLinesCount);
  // std::copy(&reflectionLines[0], &reflectionLines[0] + reflectionLinesCount, _reflectionLines);
  // reflectionLines = _reflectionLines;

  this->lastReflectionLines = std::vector<Line<T>>(lastReflectionLinesIndices.size());
  for (int i = 0; i < lastReflectionLinesIndices.size(); i++) {
    this->lastReflectionLines[i] = reflectionLines[lastReflectionLinesIndices[i]];
  }

  // std::cout << "Last reflection lines count: " << lastReflectionLinesCount << '\n';

  this->lines = reflectionLines;

  // destructors
  for (int i = 0; i < this->direction_lines.size(); i++) {
    delete MTDVectorsScaleRotationMatrices[i];
  }
  for (int i = 0; i < this->direction_lines.size(); i++) {
    delete MTMTDVectorsScaleRotationMatrices[i];
  }
}

template<typename T>
std::vector<Line<T>> FractalPart<T>::get_lines() {
  return this->lines;
}


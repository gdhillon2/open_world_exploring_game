#include "collision.h"
#include <iostream>

void CheckSpriteBorders(int maxHeight, int maxWidth, int *currentY,
                        int *currentX, int spriteHeight, int spriteWidth) {
  if (*currentX + spriteWidth > maxWidth) {
    *currentX = maxWidth - spriteWidth;
    std::cout << "Character exceeded right border\n";
  }
  if (*currentX < 0) {
    *currentX = 0;
    std::cout << "Character exceeded left border\n";
  }
  if (*currentY + spriteHeight > maxHeight) {
    *currentY = maxHeight - spriteHeight;
    std::cout << "Character exceeded bottom border\n";
  }
  if (*currentY < 0) {
    *currentY = 0;
    std::cout << "Character exceeded top border\n";
  }
}

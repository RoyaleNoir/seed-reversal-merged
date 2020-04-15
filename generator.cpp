#include "generator.h"

#define WATERFALL_Z 10
#define TREE1_X -5 // Left tree on the image
#define TREE1_Z WATERFALL_Z - 8
#define TREE1_HEIGHT 5
#define TREE2_X -3 // right tree on the image
#define TREE2_Z WATERFALL_Z + 3
#define TREE2_HEIGHT 5

bool generator::ChunkGenerator::isValidTreeSpot(int treeX, int treeZ, bool firstTreeFound, bool secondTreeFound, int waterfallX)
{
    if (treeZ >= WATERFALL_Z - 1 && treeZ <= WATERFALL_Z + 1 && treeX <= waterfallX - 3 && treeX >= waterfallX - 5)
        return true;
    if (!firstTreeFound) {
        if (treeX >= waterfallX + TREE1_X - 1 && treeX <= waterfallX + TREE1_X + 1 && treeZ >= TREE1_Z - 1 && treeZ <= TREE1_Z + 1)
            return true;
    }
    if (!secondTreeFound) {
        if (treeX >= waterfallX + TREE2_X - 1 && treeX <= waterfallX + TREE2_X + 1 && treeZ >= TREE2_Z - 1 && treeZ <= TREE2_Z + 1)
            return true;
    }
    return false;
}

bool *generator::ChunkGenerator::generateLeafPattern(random_math::JavaRand& random)
{
    bool *out = new bool[16];
    for (int32_t i = 0; i < 16; i++) {
        out[i] = random.nextInt(2) != 0;
    }
    return out;
}

int32_t generator::ChunkGenerator::checkTrees(random_math::JavaRand& random, int32_t maxTreeCount, int waterfallX)
{
    bool treesFound[2] = {false, false};
    int8_t foundTreeCount = 0;
    for (int i = 0; i <= maxTreeCount; ++i) {
        int32_t treeX = random.nextInt(16);
        int32_t treeZ = random.nextInt(16);
        int32_t height = random.nextInt(3) + 4;
        if (!treesFound[0] && treeX == waterfallX + TREE1_X && treeZ == TREE1_Z && height == TREE1_HEIGHT) {
            delete[] generator::ChunkGenerator::generateLeafPattern(random);
            foundTreeCount++;
            treesFound[0] = true;
        } else if (!treesFound[1] && treeX == waterfallX + TREE2_X && treeZ == TREE2_Z && height == TREE2_HEIGHT) {
            bool *leafPattern = generateLeafPattern(random);
            if (!leafPattern[0] && leafPattern[4]) {
                foundTreeCount++;
                treesFound[1] = true;
            } else {
                delete[] leafPattern;
                return -1;
            }
            delete[] leafPattern;
        } else {
            if (isValidTreeSpot(treeX, treeZ, treesFound[0], treesFound[1], waterfallX))
                return -1;
        }
        if (foundTreeCount == 2)
            return i;
    }
    return -1;
}

bool generator::ChunkGenerator::populate(int64_t chunkSeed, int waterfallX)
{
    random_math::JavaRand random(advance_3759.next(chunkSeed), false);

    int32_t maxBaseTreeCount = 12;
    if (random.nextInt(10) == 0)
        return false;

    int uTrees = ChunkGenerator::checkTrees(random, maxBaseTreeCount, waterfallX);
    return uTrees != -1;
}

void generator::ChunkGenerator::init()
{
    generator::ChunkGenerator::advance_3759 = random_math::JavaRand::lcg.combine(3759);
}
random_math::LCG generator::ChunkGenerator::advance_3759(1, 1, 1);

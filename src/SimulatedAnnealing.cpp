#include "SimulatedAnnealing.hpp"
#include <chrono>
#include <cmath>

SimulatedAnnealing::SimulatedAnnealing(GraphMatrix *graph) : TSPAlgorithm(graph)
{
    // TODO: improve calculation of this value
    maxNoImprovementIters = 10000 * graph->getVertexCount();
}

double SimulatedAnnealing::getAverageEdgeWeight()
{
    double numberOfEdges = graphSize * (graphSize - 1);
    int edgeSum = 0;
    for (int i = 0; i < graphSize; ++i)
    {
        for (int j = 0; j < graphSize; ++j)
        {
            if (i != j)
            {
                edgeSum += graph->getWeight(i, j);
            }
        }
    }
    return edgeSum / numberOfEdges;
}

double SimulatedAnnealing::getEdgeWeightAAD()
{
    const int numberOfEdges = graphSize * (graphSize - 1);
    const double average = getAverageEdgeWeight();

    double deviationSum = 0.0;
    for (int i = 0; i < graphSize; ++i)
    {
        for (int j = 0; j < graphSize; ++j)
        {
            if (i != j)
            {
                deviationSum += abs(average - graph->getWeight(i, j));
            }
        }
    }

    return deviationSum / numberOfEdges;
}

double SimulatedAnnealing::getInitialTemperature()
{
    double averageDeviation = getEdgeWeightAAD();
    // printf("avr abs dev: %f\n", averageDeviation);
    return 100.0 * averageDeviation;
}

Path SimulatedAnnealing::solveTSP()
{

    setInitialPath();
    this->currentPathWeight = getCurrentPathWeight();
    this->initialPathWeight = currentPathWeight;
    this->bestFoundPathWeight = currentPathWeight;
    printImprovement(0, currentPathWeight);

    // printPath(currentPath);
    // printf("Initial path weight: %i\n", currentPathWeight);

    double temperature = getInitialTemperature();

    iteration = 0;
    timer.start();
    do
    {
        temperature = temperature * coolingRate;
        // printf("temp: %f\n", temperature);

        getNextPathPermutation();

        if (transitionProbability(temperature) > randomDouble())
        {
            acceptNextPath();
        }
        ++iteration;
        ++noImprovementIters;
    } while (!endCriterionIsMet());
    // printImprovement(0, initialPathWeight);
    printPath(bestFoundPath);

    float prd = getPrd(bestFoundPathWeight);
    std::vector<int> resPath(graphSize);
    for (int i = 0; i < graphSize; ++i)
    {
        resPath[i] = bestFoundPath[i];
    }
    return Path(resPath, bestFoundPathWeight, prd);
}

void SimulatedAnnealing::getNextPathPermutation()
{
    int index1;
    int index2;
    do
    {
        index1 = randomPathIndex();
        index2 = randomPathIndex();
    } while (index1 == index2 || index1 == 0 || index2 == 0);

    switch (neighborMode)
    {
    case Swap:
        swap(index1, index2);
        break;
    case Invert:
        invert(index1, index2);
        break;
    }
}

double SimulatedAnnealing::transitionProbability(double temperature)
{
    if (nextPathWeight < currentPathWeight)
    {
        return 1.0;
    }
    else
    {
        double p = exp(((double)(currentPathWeight - nextPathWeight)) / temperature);
        // printf("probability: %f\n", p);
        return p;
    }
}

void SimulatedAnnealing::acceptNextPath()
{
    copyPath(nextPath, currentPath);
    currentPathWeight = nextPathWeight;

    if (currentPathWeight < bestFoundPathWeight)
    {
        copyPath(currentPath, bestFoundPath);
        bestFoundPathWeight = currentPathWeight;

        // printf("ItersWithoutImprovement: %i\n", itersWithoutImprovement);

        printImprovement(iteration, bestFoundPathWeight);
        noImprovementIters = 0;
    }
}

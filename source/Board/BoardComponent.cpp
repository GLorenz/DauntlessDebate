// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "Board\BoardComponent.h"

BoardComponent::BoardComponent(GameObject& gameObject, int size, sf::IntRect boardBounds)
    : GameComponent::GameComponent(gameObject)
    , m_squares(size * size)
    , m_meeples()
    , m_meepleObjects()
    , m_size(size)
    , m_boardArea(boardBounds)
    , m_clickManager(*this)
{
    sf::Vector2i squareScale(m_boardArea.width / size, m_boardArea.height / size);

    // create all squares and save in m_squares
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            sf::Vector2i pos = sf::Vector2i(m_boardArea.left + x * squareScale.x, m_boardArea.top + y * squareScale.y);

            auto newSqaureComp = gameObject.addComponent<SquareComponent>(
                Layer::BackgroundDetails, // layer
                sf::Vector2i(x,y), // board coordinates, x = col, y = row
                sf::IntRect(pos, squareScale) // absolute position
            );
            // TODO: remove this
            //std::cout << "Square at: " << pos.x << " " << pos.y << " scale " << squareScale.x << " " << squareScale.y << std::endl;
            m_squares[getVectorIdxFromRowCol(x,y)] = newSqaureComp;
        }
    }
}

void BoardComponent::update()
{
    m_clickManager.update();
}

int BoardComponent::getSize()
{
    return m_size;
}

BoardComponent::Squares BoardComponent::getNeighbourSquares(Square square)
{
    auto squareCoords = square->getBoardCoordinates();

    Squares result;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            
            if(i >= 0 && i < m_size && j >= 0 && j < m_size)
                result.push_back(getSquareAt(squareCoords.x + i, squareCoords.y + j));
        }
    }
    return result;
}

BoardComponent::Squares BoardComponent::getFreeNeighbourSquares(Square square)
{
    auto squareCoords = square->getBoardCoordinates();

    Squares result;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;

            int targX = squareCoords.x + i;
            int targY = squareCoords.y + j;
            if (targX >= 0 && targX < m_size && targY >= 0 && targY < m_size && getMeepleOnSquare(getSquareAt(targX,targY)) == nullptr)
                result.push_back(getSquareAt(targX, targY));
        }
    }
    return result;
}

std::vector<BoardComponent::MeeplePTR> BoardComponent::getMeeplesOfPlayer(bool ofPlayerOne)
{
    std::vector<MeeplePTR> playerMeeps;
    
    for (auto meep : m_meeples) {
        if (meep->isPlayerOne() == ofPlayerOne)
            playerMeeps.push_back(meep);
    }

    return playerMeeps;
}

std::vector<BoardComponent::MeeplePTR>& BoardComponent::getAllMeeples()
{
    return m_meeples;
}

BoardComponent::MeeplePTR BoardComponent::getMeepleOnSquare(Square square)
{
    // this may be a nullptr
    auto meepIt = std::find_if(
        m_meeples.begin(),
        m_meeples.end(),
        // capture position by reference (this is not a pointer)
        [&square](BoardComponent::MeeplePTR mPtr) { return mPtr->getCurrentSquare() == square; }
    );
    return meepIt == m_meeples.end() ? nullptr : *meepIt;
}

void BoardComponent::addMeeple(MeeplePTR meeple)
{
    m_meeples.push_back(meeple);
}

void BoardComponent::addMeeple(std::shared_ptr<GameObject> meepleObject)
{
    m_meepleObjects.push_back(meepleObject);

    auto meepleptr = meepleObject->getComponent<Meeple>();
    if (meepleptr != nullptr)
        addMeeple(meepleptr);
}

void BoardComponent::countMeeples(int& meeplesP1, int& meeplesP2)
{
    meeplesP1 = 0;
    meeplesP2 = 0;

    for (auto meep : m_meeples) {
        if (meep->isPlayerOne())
            ++meeplesP1;
        else
            ++meeplesP2;
    }
}

BoardClickManager& BoardComponent::getClickManager()
{
    return m_clickManager;
}

sf::IntRect BoardComponent::getBoardRect()
{
    return m_boardArea;
}

void BoardComponent::setMeeplesActive(bool toActive)
{
    for (auto meep : m_meepleObjects) {
        meep->setActive(toActive);
    }
}

bool BoardComponent::bothHaveMeeples()
{
    bool p1hasMeeples = false;
    bool p2hasMeeples = false;
    for (auto meep : m_meeples)
    {
        if (meep->isPlayerOne()) p1hasMeeples = true;
        else p2hasMeeples = true;

        if (p1hasMeeples && p2hasMeeples) return true;
    }
    
    return false;
}

int BoardComponent::getVectorIdxFromRowCol(int column, int row)
{
    return column + m_size * row;
}

BoardComponent::Square BoardComponent::getSquareAt(int column, int row)
{
    if (column >= 0 && row >= 0 && column < m_size && row < m_size) {
        return m_squares[getVectorIdxFromRowCol(column, row)];
    }
    else
        return nullptr;
}

BoardComponent::Square BoardComponent::getSquareAt(sf::Vector2i absolutePosition)
{
    // TODO: remove this
    // std::cout << "find square at " << absolutePosition.x << " " << absolutePosition.y << std::endl;
    // this may be a nullptr
    auto squareIt = std::find_if(
        m_squares.begin(),
        m_squares.end(),
        // capture position by reference (this is not a pointer)
        [&absolutePosition](BoardComponent::Square sPtr) { return sPtr->getBounds().contains(absolutePosition); }
    );
    
    return squareIt == m_squares.end() ? nullptr : *squareIt;
}
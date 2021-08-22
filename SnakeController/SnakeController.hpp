#pragma once

#include <list>
#include <memory>
#include <functional>

#include "IEventHandler.hpp"
#include "SnakeInterface.hpp"
#include <stdexcept>

class Event;
class IPort;

namespace Snake
{
struct ConfigurationError : std::logic_error
{
    ConfigurationError();
};

struct UnexpectedEventException : std::runtime_error
{
    UnexpectedEventException();
};



struct Body
{
    struct Segment
    {
        int x;
        int y;
    };

    std::list<Segment> m_segments;

    bool isSegmentAtPosition(int x, int y) const;
    Segment calculateNewHead(Direction m_currentDirection) const;
    void updateSegmentsIfSuccessfullMove(Segment const& newHead);
    void addHeadSegment(Segment const& newHead, IPort& m_displayPort);
    void removeTailSegmentIfNotScored(Segment const& newHead);
    void removeTailSegment(IPort& m_displayPort);
};

struct Map
{
    std::pair<int, int> m_mapDimension;
    std::pair<int, int> m_foodPosition;

    bool isPositionOutsideMap(int x, int y) const;

    void updateFoodPosition(int x, int y, std::function<void()> clearPolicy);
    void sendClearOldFood();
    void sendPlaceNewFood(int x, int y);
};

struct Ports
{
    Ports(IPort& _displayPort, IPort& _foodPort, IPort& _scorePort);

    IPort& m_displayPort;
    IPort& m_foodPort;
    IPort& m_scorePort;
};

class Controller : public IEventHandler
{
public:
    Controller(IPort& p_displayPort, IPort& p_foodPort, IPort& p_scorePort, std::string const& p_config);

    Controller(Controller const& p_rhs) = delete;
    Controller& operator=(Controller const& p_rhs) = delete;

    void receive(std::unique_ptr<Event> e) override;

private:

    static Ports m_gamePorts;
    static Body m_snakeBody;
    static Map m_gameMap;

    Direction m_currentDirection;

    void handleTimeoutInd();
    void handleDirectionInd(std::unique_ptr<Event>);
    void handleFoodInd(std::unique_ptr<Event>);
    void handleFoodResp(std::unique_ptr<Event>);
    void handlePauseInd(std::unique_ptr<Event>);

    bool m_paused;
};

} // namespace Snake

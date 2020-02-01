#include "Point.h"

Point::Point(int x, int y) : x(x), y(y) {}

int Point::getX() const {
    return x;
}

int Point::getY() const {
    return y;
}

bool Point::operator==(Point other) {
    return this->x == other.x && this->y == other.y;
}

bool Point::isLeftNeighbor(Point other) {
    return this->x == other.getX() && this->y == (other.getY() -1);
}

bool Point::isRightNeighbor(Point other) {
    return this->x == other.getX() && this->y == (other.getY() +1);
}

bool Point::isUpNeighbor(Point other) {
    return this->x == (other.getX() - 1) && this->y == other.getY();
}

bool Point::isDownNeighbor(Point other) {
    return this->x == (other.getX() + 1) && this->y == other.getY();
}

string Point::move(Point other) {
    if (this->x == other.getX() && this->y > other.getY()) {
        return "R";
    }
    if (this->x == other.getX() && this->y < other.getY()) {
        return "L";
    }
    if (this->x > other.getX() && this->y == other.getY()) {
        return "D";
    }
    if (this->x < other.getX() && this->y == other.getY()) {
        return "U";
    }
}
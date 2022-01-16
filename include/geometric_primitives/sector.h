#ifndef GEOMETRIC_PRIMITIVES_SECTOR_H_
#define GEOMETRIC_PRIMITIVES_SECTOR_H_

#include <string>
#include <vector>
#include <utility>

#include "geometric_primitives/point2D.h"
#include "geometric_primitives/vector2D.h"
#include "geometric_primitives/line2D.h"
#include "geometric_primitives/ray2D.h"
#include "geometric_primitives/segment2D.h"
#include "geometric_primitives/circle.h"


class Sector: public Circle {
private:
    Point2D original_bisector_unit_point;
    double angle;
    double half_angle;
    double orientation_angle;
    Point2D bisector_unit_point;
    Vector2D bisector_unit_vector;
    Ray2D bisector_ray;
    Point2D endpoint1;
    Point2D endpoint2;
    Point2D midpoint;
    Segment2D boundary1;
    Segment2D boundary2;
    
    void init(const Point2D& center, double radius, double angle, double orientation_angle=0);
    void computeMidpointAndEndpoints();
    std::pair<Point2D, Point2D> getPointsInsideSector(const std::pair<Point2D, Point2D>& pair_points) const;

public:
    Sector();
    Sector(const Point2D& center, double radius, double angle, double orientation_angle=0);
    virtual void setRadius(double radius) override;
    void orientWithAngle(double orientation_angle);
    Point2D getOriginalBisectorUnitPoint() const;
    Point2D getBisectorUnitPoint() const;
    double getAngle() const;
    double getOrientationAngle() const;
    Point2D getEndpoint1() const;
    Point2D getEndpoint2() const;
    Point2D getMidPoint() const;
    Vector2D getBisectorUnitVector() const;
    Ray2D getBisectorRay() const;
    Segment2D getBoundarySegment1() const;
    Segment2D getBoundarySegment2() const;

    bool passesThroughPoint2D(const Point2D& p) const;
    bool containsPoint2D(const Point2D& p) const override;
    std::vector<Point2D> findIntersectionsSectorWithLine2D(const Line2D& line2D) const;
    std::vector<Point2D> findIntersectionsSectorWithRay2D(const Ray2D& ray2D) const;
    std::vector<Point2D> findIntersectionsSectorWithSegment2D(const Segment2D& segment2D) const;
    bool sectorIntersectsWithCircle(const Circle& circle) const;
    std::vector<Point2D> findIntersectionsSectorWithCircle(const Circle& circle) const;

    bool arcIntersectsWithLine2D(const Line2D& line2D) const;
    std::vector<Point2D> findIntersectionsArcWithLine2D(const Line2D& line2D) const;
    bool arcIntersectsWithRay2D(const Ray2D& ray2D) const;
    std::vector<Point2D> findIntersectionsArcWithRay2D(const Ray2D& ray2D) const;
    bool arcIntersectsWithSegment2D(const Segment2D& segment2D) const;
    std::vector<Point2D> findIntersectionsArcWithSegment2D(const Segment2D& segment2D) const;
    bool arcIntersectsWithCircle(const Circle& circle) const;
    std::vector<Point2D> findIntersectionsArcWithCircle(const Circle& circle) const;

    bool arcIntersectsWithArc(const Sector& other) const;
    std::vector<Point2D> findIntersectionsArcWithArc(const Sector& other) const;
    bool sectorIntersectsWithArc(const Sector& other) const;
    std::vector<Point2D> findIntersectionsSectorWithArc(const Sector& other) const;
    bool arcIntersectsWithSector(const Sector& other) const;
    std::vector<Point2D> findIntersectionsArcWithSector(const Sector& other) const;
    bool sectorIntersectsWithSector(const Sector& other) const;
    std::vector<Point2D> findIntersectionsSectorWithSector(const Sector& other) const;

    double findPossibleOrientAngleToCoverPoints2D(const std::vector<Point2D>& points) const;
    bool canOrientToCoverPoint2D(const Point2D& p) const;
    double orientToCoverPoint2D(const Point2D& p);
    bool canOrientToCoverPoints2D(const std::vector<Point2D>& points) const;
    double orientToCoverPoints2D(const std::vector<Point2D>& points);

    bool orientBoundaryPassingThroughPointAndCoverAnotherPoint(const Point2D& a, const Point2D& b);

    virtual std::string toString() const override;
    friend std::ostream& operator<<(std::ostream& os, const Circle& circle);
};

#endif //GEOMETRIC_PRIMITIVES_SECTOR_H_
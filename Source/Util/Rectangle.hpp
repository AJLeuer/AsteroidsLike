//
//  Rectangle.hpp
//  AsteroidsLike
//
//  Created by Adam James Leuer on 5/21/15.
//  Copyright (c) 2015 Adam James Leuer. All rights reserved.
//

#ifndef AsteroidsLike_Rectangle_hpp
#define AsteroidsLike_Rectangle_hpp

#include "Vect.hpp"
#include "Size.hpp"
#include "Line.hpp"
#include <array>

using namespace std ;

struct InitializeFromTopLeftCoordinates {
	inline constexpr InitializeFromTopLeftCoordinates() {}
} ;

struct InitializeFromCenterCoordinates {
	inline constexpr InitializeFromCenterCoordinates() {}
} ;

template <typename PositionUnderlyingType, typename SizeUnderlyingType>
struct Rectangle {
	
protected:
	
	/**
	 * The top left corner of the rectangle
	 */
	Vect<PositionUnderlyingType> vertex0 ;
	
	/**
	 * The top right corner of the rectangle
	 */
	Vect<PositionUnderlyingType> vertex1 ;
	
	/**
	 * The bottom left corner of the rectangle
	 */
	Vect<PositionUnderlyingType> vertex2 ;
	
	/**
	 * The bottom right corner of the rectangle
	 */
	Vect<PositionUnderlyingType> vertex3 ;
	
	/**
	 * The center of the rectangle
	 */
	Vect<PositionUnderlyingType> center ;
	
	/**
	 * For convenience and easier looping
	 */
	array< Vect<PositionUnderlyingType> *, 4> allPoints { & vertex0, & vertex1, & vertex2, & vertex3 } ;
	
	Vect<PositionUnderlyingType> & calculateCenter() const ;
	
public:
	
	static bool detectCollision(const Rectangle & first, const Rectangle & second) ;
	
	Rectangle() :
		vertex0(0, 0),
		vertex1(0, 0),
		vertex2(0, 0),
		vertex3(0, 0),
		center(0, 0)
	{
		
	}
	
	Rectangle(const Vect<PositionUnderlyingType> v0, const Vect<PositionUnderlyingType> v1,
			  const Vect<PositionUnderlyingType> v2, const Vect<PositionUnderlyingType> v3) :
		vertex0(v0),
		vertex1(v1),
		vertex2(v2),
		vertex3(v3),
		center(calculateCenter())
	{
		
	}
	
	Rectangle(const Vect<PositionUnderlyingType> & topLeftCorner, const Size<SizeUnderlyingType> & widthHeight,
			  InitializeFromTopLeftCoordinates initializationMethod) :
		vertex0(topLeftCorner),
		vertex1((topLeftCorner.getX() + widthHeight.getWidth()), topLeftCorner.getY()),
		vertex2(topLeftCorner.getX(), (topLeftCorner.getY() + widthHeight.getHeight())),
		vertex3((topLeftCorner.getX() + widthHeight.getWidth()), (topLeftCorner.getY() + widthHeight.getHeight())),
		center(calculateCenter())
	
	{
		
	}
	
	Rectangle(const Vect<PositionUnderlyingType> & center, const Size<SizeUnderlyingType> & widthHeight,
			  InitializeFromCenterCoordinates initializationMethod) :
		vertex0((center.getX() - (widthHeight.getWidth() / 2)), (center.getY() - (widthHeight.getHeight() / 2))),
		vertex1((center.getX() + (widthHeight.getWidth() / 2)), (center.getY() - (widthHeight.getHeight() / 2))),
		vertex2((center.getX() - (widthHeight.getWidth() / 2)), (center.getY() + (widthHeight.getHeight() / 2))),
		vertex3((center.getX() + (widthHeight.getWidth() / 2)), (center.getY() + (widthHeight.getHeight() / 2))),
		center(center)
	{
		
	}
	
	Rectangle(const Rectangle & other) :
		vertex0(other.vertex0),
		vertex1(other.vertex1),
		vertex2(other.vertex2),
		vertex3(other.vertex3),
		center(other.center)
	{
		
	}
	
	~Rectangle() {}
	
	const Vect<PositionUnderlyingType> getCenter() const { return center ; }
	
	inline const Line<PositionUnderlyingType> firstLineSegment() const { return Line<PositionUnderlyingType>(vertex0, vertex1) ; }
	
	inline const Line<PositionUnderlyingType> secondLineSegment() const { return Line<PositionUnderlyingType>(vertex1, vertex2) ; }
	
	inline const Line<PositionUnderlyingType> thirdLineSegment() const { return Line<PositionUnderlyingType>(vertex2, vertex3) ; }
	
	inline const Line<PositionUnderlyingType> fourthLineSegment() const { return Line<PositionUnderlyingType>(vertex3, vertex0) ; }
	
	inline const array< const Line<PositionUnderlyingType>, 4> componentLineSegments() const {
		return array< const Line<PositionUnderlyingType>, 4> { firstLineSegment(), secondLineSegment(), thirdLineSegment(), fourthLineSegment() } ;
	}
	
	void rotate(const Angle angle) ;
	
};

template <typename PositionUnderlyingType, typename SizeUnderlyingType>
Vect<PositionUnderlyingType> & Rectangle<PositionUnderlyingType, SizeUnderlyingType>::calculateCenter() const {
	auto x_center = (vertex0.getX() + vertex3.getX()) / 2 ;
	auto y_center = (vertex0.getY() + vertex3.getY()) / 2 ;
	this->center = Vect<PositionUnderlyingType> { x_center, y_center } ;
	return center ;
}

template <typename PositionUnderlyingType, typename SizeUnderlyingType>
bool Rectangle<PositionUnderlyingType, SizeUnderlyingType>::detectCollision(const Rectangle & first, const Rectangle & second) {
	auto firstRectLineSegments = first.componentLineSegments() ;
	auto secondRectLineSegments = second.componentLineSegments() ;
	
	auto totalAxes = firstRectLineSegments.size() + secondRectLineSegments.size() ;
	
	for (auto i = 0 ; i < totalAxes ; i++) {
		
		const Line<PositionUnderlyingType> * currentLine ;
		
		if (i < firstRectLineSegments.size()) {
			currentLine = & firstRectLineSegments[i] ;
		}
		else {
			currentLine = & secondRectLineSegments[i - secondRectLineSegments.size()] ;
		}
		
		auto axis = currentLine->vectorize().first ;
		axis.rotate(90.0) ;
		
		Vect<PositionUnderlyingType> firstRectMinimum {0, 0} ;
		Vect<PositionUnderlyingType> firstRectMaximum {0, 0} ;
		
		Vect<PositionUnderlyingType> secondRectMinimum {0, 0} ;
		Vect<PositionUnderlyingType> secondRectMaximum {0, 0} ;
		
		for (auto j = 0; (j < first.allPoints.size()) && (j < second.allPoints.size()) ; j++) {
			
			Vect<PositionUnderlyingType> * currentVertexFromFirst = first.allPoints.at(j) ;
			Vect<PositionUnderlyingType> * currentVertexFromSecond = second.allPoints.at(j) ;
			
			auto currentProjectedFirstRectVertex = axis.projectOnto(* currentVertexFromFirst) ;
			auto currentProjectedSecondRectVertex = axis.projectOnto(* currentVertexFromSecond) ;
			
			
			if (j == 0) {
				firstRectMinimum = currentProjectedFirstRectVertex ;
				firstRectMaximum = currentProjectedFirstRectVertex ;
				
				secondRectMinimum = currentProjectedSecondRectVertex ;
				secondRectMaximum = currentProjectedSecondRectVertex ;
			}
			
			else {
				
				if (currentProjectedFirstRectVertex < firstRectMinimum) {
					firstRectMinimum = currentProjectedFirstRectVertex ;
				}
				
				else if (currentProjectedFirstRectVertex > firstRectMaximum) {
					firstRectMaximum = currentProjectedFirstRectVertex ;
				}
				
				if (currentProjectedSecondRectVertex < secondRectMinimum) {
					secondRectMinimum = currentProjectedSecondRectVertex ;
				}
				
				else if (currentProjectedSecondRectVertex > secondRectMaximum) {
					secondRectMaximum = currentProjectedSecondRectVertex ;
				}
			
			}
				
		}
		
		if ((secondRectMinimum > firstRectMaximum) || (secondRectMaximum < firstRectMinimum)) {
			//then there's a gap (no overlap) on this axis between the projected points of the two
			//rectangles, so no collision is possible. Return immediately.
			return false ;
			
		}
		
	}
	
	//if we've gotten to this point, then we've looped through all the sides of both rectangles, and projected all of the points
	//from both onto the axes perpendicular to each side. And we found no gaps. This means the rects intersect somewhere, so we
	//return true
	return true ;
}


template <typename PositionUnderlyingType, typename SizeUnderlyingType>
void Rectangle<PositionUnderlyingType, SizeUnderlyingType>::rotate(const Angle angle) {
	
	for (auto i = 0 ; i < allPoints.size() ; i++) {
		allPoints[i]->rotateAround(angle, center) ;
	}
	
}









#endif

//
//  Line.hpp
//  AsteroidsLike
//
//  Created by Adam James Leuer on 5/27/15.
//  Copyright (c) 2015 Adam James Leuer. All rights reserved.
//

#ifndef AsteroidsLike_Line_hpp
#define AsteroidsLike_Line_hpp

#include <tuple>

#include "Vect.hpp"

using namespace std ;

template <typename N>
struct Line {
	
	Vect<N> startPoint ;
	
	Vect<N> endPoint ;
	
	Line(const Vect<N> & start, const Vect<N> & end) : startPoint(start), endPoint(end) {}
	
	N length() const ;
	
	Vect<N> projectOnto(const Vect<N> & vect) ;
	
	pair<Vect<N>, Vect<N>> vectorize() const ;
	
} ;

template <typename N>
N Line<N>::length() const {
	return Vect<N>::distanceBetween(startPoint, endPoint) ;
}

template <typename N>
Vect<N> Line<N>::projectOnto(const Vect<N> & vect) {
	const auto vectorizedLine = this->vectorize().first ;
	N dotProduct1 = Vect<N>::dotProduct(vect, vectorizedLine) ;
	N dotProductDivisor = Vect<N>::dotProduct(vectorizedLine, vectorizedLine) ;
	N divResult = (dotProduct1 / dotProductDivisor) ;
	const Vect<N> result = divResult * vectorizedLine ;
	return result ;
}


template <typename N>
pair<Vect<N>, Vect<N>> Line<N>::vectorize() const {
	auto x_diff = endPoint.getX() - startPoint.getX() ;
	auto y_diff = endPoint.getY() - startPoint.getY() ;
	
	auto x_diff_reverse = startPoint.getX() - endPoint.getX() ;
	auto y_diff_reverse = startPoint.getY() - endPoint.getY() ;
	
	Vect<N> v1 {y_diff, x_diff} ;
	Vect<N> v2 {y_diff_reverse, x_diff_reverse} ;
	
	v1.normalize() ;
	v2.normalize() ;
	
	return std::pair<Vect<N>, Vect<N>> {v1, v2} ;
}


#endif

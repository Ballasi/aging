#ifndef VEC2_H_
#define VEC2_H_

/*!
 * \file Vec2.h
 * \brief A 2 dimentionnal value
 *
 */

#include <cstddef>
#include "BigInt.h"

/*!
 * \class Vec2
 * \brief A 2 dimentionnal value
 *
 * It can be used to represent position or a size
 * with an arbitrary large pair of integers
 */
class Vec2 {
public:
  // Inner data
  /*!
   * X / horizontal componant
   */
  BigInt x;

  /*!
   * Y / vectical componant
   */
  BigInt y;


  // Constructors
  /*!
   * \brief Default contructor
   *
   * Creates the (0, 0) vector
   */
  Vec2() = default;

  /*!
   * \brief Creates the (2^level, 2^level)
   *
   * Useful in HashUniverse
   *
   * __Example:__
   * \code
   * Vec2 vec(3);
   * assert(vec.x == 8) // 8 = 2 ^ 3
   * assert(vec.y == 8) // 8 = 2 ^ 3
   * \endcode
   *
   * \param level: 2's exponent of both componant
   */
  explicit Vec2(const size_t level);

  /*!
   * \brief Constructor
   *
   * Create a Vec2 using it's two componants.
   * \param x: x componant
   * \param y: y componant
   */
  Vec2(const BigInt &x, const BigInt &y);


  // Equality operators
  /*!
   * \brief Equality operator
   *
   * \param other: value to compare with
   * \return true if they are equal
   */
  bool operator==(const Vec2 &other) const;

  /*!
   * \brief Inequality operator
   *
   * \param other: value to compare with
   * \return true if they are different
   */
  bool operator!=(const Vec2 &other) const;

  // Inclusion operators
  /*!
   * \brief Strict inclusion operator
   *
   * __Example__:
   * \code
   * if (v1 < v2) {
   * 	// v1.x < v2.x
   * 	// v1.y < v2.y
   * }
   * \endcode
   *
   * \param other: compared vector
   * \return true if strictly smaller than `other`
   */
  bool operator< (const Vec2 &other) const;

  /*!
   * \brief Inclusion operator
   *
   * __Example__:
   * \code
   * if (v1 <= v2) {
   * 	// v1.x <= v2.x
   * 	// v1.y <= v2.y
   * }
   * \endcode
   *
   * \param other: compared vector
   * \return true if smaller or identical to the other
   */
  bool operator<=(const Vec2 &other) const;

  /*!
   * \brief Reverse strict inclusion operator
   *
   * Same as Vec2::operator< but reversed
   *
   * __Example__:
   * \code
   * if (v1 > v2) {
   * 	// v1.x > v2.x
   * 	// v1.y > v2.y
   * }
   * \endcode
   *
   * \param other: compared vector
   * \return true if bigger than `other`
   */
  bool operator> (const Vec2 &other) const;

  /*!
   * \brief Reverse inclusion operator
   *
   * Same as Vec2::operator<= but reversed
   *
   * __Example__:
   * \code
   * if (v1 >= v2) {
   * 	// v1.x >= v2.x
   * 	// v1.y >= v2.y
   * }
   * \endcode
   *
   * \param other: compared vector
   * \return true if bigger or identical to the other
   */
  bool operator>=(const Vec2 &other) const;

  // Sum operators
  /*!
   * \brief Addition operator
   *
   * Adds both componants
   *
   * \param other: added vector
   * \return the sum of each componant
   */
  Vec2 operator+ (const Vec2 &other) const;

  /*!
   * \brief Substraction operator
   *
   * Substracts both componants
   *
   * \param other: subscrated vector
   * \return the difference of each componant
   */
  Vec2 operator- (const Vec2 &other) const;

  /*!
   * \brief Inplace addition operator
   *
   * Same as Vec2::operator+ but in place
   *
   * Adds `other` to caller in place
   * **Using this is faster than using +**
   *
   * \param other: added vector
   */
  void operator+=(const Vec2 &other);

  /*!
   * \brief Inplace substraction operator
   *
   * Same as Vec2::operator- but in place
   *
   * Subtracts `other` to caller in place
   * **Using this is faster than using -**
   *
   * \param other: substracted vector
   */
  void operator-=(const Vec2 &other);


  // Scaling operators
  /*!
   * \brief Fast downscaling operator
   *
   * Left bitshift on each componant
   *
   * \param scale: scale factor
   * \return downscaled vector
   */
  Vec2 operator<< (const size_t scale) const;

  /*!
   * \brief Fast upscaling operator
   *
   * Right bitshift on each componant
   *
   * \param scale: scale factor
   * \return upscaled vector
   */
  Vec2 operator>> (const size_t scale) const;

  /*!
   * \brief Fast in place downscaling operator
   *
   * Same as Vec2::operator<< but in place
   *
   * Inplace left bitshift on each componant
   *
   * \param scale: scale factor
   */
  void operator<<=(const size_t scale);

  /*!
   * \brief Fast in place upscaling operator
   *
   * Same as Vec2::operator>> but in place
   *
   * Inplace right bitshift on each componant
   *
   * \param scale: scale factor
   */
  void operator>>=(const size_t scale);
};

#endif // VEC2_H_

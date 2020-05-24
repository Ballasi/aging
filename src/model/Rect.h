#ifndef RECT_H_
#define RECT_H_

/*!
 * \file Rect.h
 * \brief Describes a section of space
 */

#include <cstddef>
#include <string>

#include "BigInt.h"
#include "Vec2.h"

/*!
 * \class Rect
 * \brief Description of a sub section of space.
 */
class Rect {
public:
  // Inner data
  Vec2 top_left;
  Vec2 size;

  // Constructors
  /*!
   * \brief Default constructor
   *
   * Creates the Rect of size 0
   * and coordonnate (0, 0)
   */
  Rect() = default;

  /*!
   * \brief Constructor
   *
   * \param top_left: top left coordonnate
   * \param size: size
   */
  Rect(const Vec2 &top_left, const Vec2 &size);

  /*!
   * \brief Constructor with level
   *
   * Creates a Rect size (2^level, 2^level)
   *
   * __Example:__
   * \code
   * Rect rect({0, 0}, 3);
   * assert(rect.size.x == 8) // 8 = 2 ^ 3
   * assert(rect.size.y == 8) // 8 = 2 ^ 3
   * \endcode
   *
   * \param top_left: top left coordonnate
   * \param level: size level
   */
  Rect(const Vec2 &top_left, size_t level);

  // Equality operators

  /*!
   * \brief Equality operator
   *
   * \param other: value to compare with
   * \return true if they are equal
   */
  bool operator==(const Rect &rect) const;

  /*!
   * \brief Inequality operator
   *
   * \param other: value to compare with
   * \return true if they are different
   */
  bool operator!=(const Rect &rect) const;

  // Collides operator
  /*!
   * \brief Point collision operator
   *
   * \param point: point to check collision with
   * \return true if point is in this rect
   */
  bool operator&(const Vec2 &point) const;

  /*!
   * \brief Rectangle overlap operator
   *
   * \param rect: rect to check overlap with
   * \return true if rects overlap
   */
  bool operator&(const Rect &rect) const;

  // Does not collide operator
  /*!
   * \brief Point NO collision operator
   *
   * \param point: point to check collision with
   * \return true if point is NOT in this rect
   */
  bool operator|(const Vec2 &point) const;

  /*!
   * \brief Rect NO overlap operator
   *
   * \param point: rect to check overlap with
   * \return true if rect does NOT overlap this rect
   */
  bool operator|(const Rect &rect) const;

  // Move operator
  /*!
   * \brief Translation operator
   *
   * Changes top left without touching the size
   * \param translation: amount to translate by
   * \return the translated rectangle
   */
  Rect operator+(const Vec2 &translation) const;

  /*!
   * \brief Oposite translation operator
   *
   * Changes top left without touching the size
   * \param translation: oposite amount to translate by
   * \return the translated rectangle
   */
  Rect operator-(const Vec2 &translation) const;

  /*!
   * \brief In place translation operator
   *
   * Changes top left in place without touching size
   * \param translation: amount to translate in place by
   */
  void operator+=(const Vec2 &translation);

  /*!
   * \brief In place oposite translation operator
   *
   * Changes top left in place without touching size
   * \param translation: oposite amount to translate in place by
   */
  void operator-=(const Vec2 &translation);

  // Scaling operators
  /*!
   * \brief Up scale operator
   *
   * Multiplies size by 2
   *
   * \param scale: number to mulitiply size by 2 
   * \return scaled up 
   */
  Rect operator<<(const size_t scale) const;

  /*!
   * \brief down scale operator
   *
   * Divides size by 2
   *
   * \param scale: number to divide size by 2 
   * \return scaled up 
   */
  Rect operator>>(const size_t scale) const;

  /*!
   * \brief Inplace up scale operator
   *
   * Multiplies in place size by 2
   * Same as Rect::operator<< but in place
   *
   * \param scale: number to mulitiply in place size by 2 
   */
  void operator<<=(const size_t scale);

  /*!
   * \brief Inplace down scale operator
   *
   * Divides in place size by 2
   * Same as Rect::operator>> but in place
   *
   * \param scale: number to divide in place size by 2 
   */
  void operator>>=(const size_t scale);

  // Getters
  /*!
   * \brief X position getter
   */
  const BigInt &x() const;

  /*!
   * \brief Y position getter
   */
  const BigInt &y() const;

  /*!
   * \brief width getter
   */
  const BigInt &width() const;

  /*!
   * \brief height getter
   */
  const BigInt &height() const;

  // Pseudo getters
  /*!
   * \brief caculates the bottom right position
   */
  const Vec2 bottom_right() const;

  // Pseudo setters
  /*!
   * \brief Reference to X coordonnate
   */
  BigInt &x();

  /*!
   * \brief Reference to Y coordonnate
   */
  BigInt &y();

  /*!
   * \brief Reference to width
   */
  BigInt &width();

  /*!
   * \brief Reference to height
   */
  BigInt &height();
};

#endif // RECT_H_

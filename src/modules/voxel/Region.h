/**
 * @file
 */

#pragma once

#include <glm/fwd.hpp>
#include "core/String.h"
#include <stdint.h>

namespace math {
class Random;
}

namespace voxel {

/** Represents a part of a Volume.
 *
 *  Many operations in PolyVox are constrained to only part of a volume. For example, when running the surface extractors
 *  it is unlikely that you will want to run it on the whole volume at once, as this will give a very large mesh which may
 *  be too much to render. Instead you will probably want to run a surface extractor a number of times on different parts
 *  of the volume, there by giving a number of meshes which can be culled and rendered separately.
 *
 *  The Region class is used to define these parts (regions) of the volume. Essentially it consists of an upper and lower
 *  bound which specify the range of voxels positions considered to be part of the region. Note that these bounds are
 *  <em>inclusive</em>.
 *
 *  As well as the expected set of getters and setters, this class also provide utility functions for increasing and decreasing
 *  the size of the Region, shifting the Region in 3D space, testing whether it contains a given position, enlarging it so that
 *  it does contain a given position, cropping it to another Region, and various other utility functions.
 *
 *  @note The dimensions of a region can be measured either in voxels or in cells. See the manual for more information
 *  about these definitions.
 *
 */
class Region {
public:
	/// Constructor
	constexpr Region();
	/// Constructor
	Region(const glm::ivec3& v3dLowerCorner, const glm::ivec3& v3dUpperCorner);
	/// Constructor
	constexpr Region(int32_t iLowerX, int32_t iLowerY, int32_t iLowerZ, int32_t iUpperX, int32_t iUpperY, int32_t iUpperZ);
	constexpr Region(int mins, int maxs);

	/// A Region with the lower corner set as low as possible and the upper corner set as high as possible.
	static const Region MaxRegion;
	static const Region InvalidRegion;

	/// Equality Operator.
	bool operator==(const Region& rhs) const;
	/// Inequality Operator.
	bool operator!=(const Region& rhs) const;

	/// Moves the Region by the amount specified.
	Region& operator+=(const glm::ivec3& v3dAmount);

	/// Gets the 'x' position of the centre.
	int32_t getCentreX() const;
	/// Gets the 'y' position of the centre.
	int32_t getCentreY() const;
	/// Gets the 'z' position of the centre.
	int32_t getCentreZ() const;
	/// Gets the 'x' position of the centre.
	float getCentreXf() const;
	/// Gets the 'y' position of the centre.
	float getCentreYf() const;
	/// Gets the 'z' position of the centre.
	float getCentreZf() const;
	/// Gets the 'x' position of the lower corner.
	int32_t getLowerX() const;
	/// Gets the 'y' position of the lower corner.
	int32_t getLowerY() const;
	/// Gets the 'z' position of the lower corner.
	int32_t getLowerZ() const;
	/// Gets the 'x' position of the upper corner.
	int32_t getUpperX() const;
	/// Gets the 'y' position of the upper corner.
	int32_t getUpperY() const;
	/// Gets the 'z' position of the upper corner.
	int32_t getUpperZ() const;

	/// Gets the centre of the region
	glm::ivec3 getCentre() const;
	glm::vec3 getCentref() const;
	/// Gets the position of the lower corner.
	glm::ivec3 getLowerCorner() const;
	/// Gets the position of the upper corner.
	glm::ivec3 getUpperCorner() const;

	glm::vec3 getLowerCornerf() const;
	glm::vec3 getUpperCornerf() const;

	glm::ivec3 getRandomPosition(math::Random& random) const;

	/// Gets the width of the region measured in voxels.
	int32_t getWidthInVoxels() const;
	/// Gets the height of the region measured in voxels.
	int32_t getHeightInVoxels() const;
	/// Gets the depth of the region measured in voxels.
	int32_t getDepthInVoxels() const;
	/// Gets the dimensions of the region measured in voxels.
	glm::ivec3 getDimensionsInVoxels() const;

	/// Gets the width of the region measured in cells.
	int32_t getWidthInCells() const;
	/// Gets the height of the region measured in cells.
	int32_t getHeightInCells() const;
	/// Gets the depth of the region measured in cells.
	int32_t getDepthInCells() const;
	/// Gets the dimensions of the region measured in cells.
	glm::ivec3 getDimensionsInCells() const;

	/// Sets the 'x' position of the lower corner.
	void setLowerX(int32_t iX);
	/// Sets the 'y' position of the lower corner.
	void setLowerY(int32_t iY);
	/// Sets the 'z' position of the lower corner.
	void setLowerZ(int32_t iZ);
	/// Sets the 'x' position of the upper corner.
	void setUpperX(int32_t iX);
	/// Sets the 'y' position of the upper corner.
	void setUpperY(int32_t iY);
	/// Sets the 'z' position of the upper corner.
	void setUpperZ(int32_t iZ);

	glm::ivec3 moveInto(int32_t x, int32_t y, int32_t z) const;

	/// Sets the position of the lower corner.
	void setLowerCorner(const glm::ivec3& v3dLowerCorner);
	/// Sets the position of the upper corner.
	void setUpperCorner(const glm::ivec3& v3dUpperCorner);

	/// Tests whether the given point is contained in this Region.
	bool containsPoint(float fX, float fY, float fZ, float boundary = 0.0f) const;
	/// Tests whether the given point is contained in this Region.
	bool containsPoint(const glm::vec3& pos, float boundary = 0.0f) const;
	/// Tests whether the given point is contained in this Region.
	bool containsPoint(int32_t iX, int32_t iY, int32_t iZ, uint8_t boundary = 0) const;
	/// Tests whether the given point is contained in this Region.
	bool containsPoint(const glm::ivec3& pos, uint8_t boundary = 0) const;
	/// Tests whether the given position is contained in the 'x' range of this Region.
	bool containsPointInX(float pos, float boundary = 0.0f) const;
	/// Tests whether the given position is contained in the 'x' range of this Region.
	bool containsPointInX(int32_t pos, uint8_t boundary = 0) const;
	/// Tests whether the given position is contained in the 'y' range of this Region.
	bool containsPointInY(float pos, float boundary = 0.0f) const;
	/// Tests whether the given position is contained in the 'y' range of this Region.
	bool containsPointInY(int32_t pos, uint8_t boundary = 0) const;
	/// Tests whether the given position is contained in the 'z' range of this Region.
	bool containsPointInZ(float pos, float boundary = 0.0f) const;
	/// Tests whether the given position is contained in the 'z' range of this Region.
	bool containsPointInZ(int32_t pos, uint8_t boundary = 0) const;

	/// Tests whether the given Region is contained in this Region.
	bool containsRegion(const Region& reg, uint8_t boundary = 0) const;

	/// Enlarges the Region so that it contains the specified position.
	void accumulate(int32_t iX, int32_t iY, int32_t iZ);
	/// Enlarges the Region so that it contains the specified position.
	void accumulate(const glm::ivec3& v3dPos);
	Region accumulateCopy(const glm::ivec3& v3dPos) const;

	/// Enlarges the Region so that it contains the specified Region.
	void accumulate(const Region& reg);
	Region accumulateCopy(const Region& reg) const;

	/// Crops the extents of this Region according to another Region.
	void cropTo(const Region& other);

	/// Grows this region by the amount specified.
	void grow(int32_t iAmount);
	/// Grows this region by the amounts specified.
	void grow(int32_t iAmountX, int32_t iAmountY, int32_t iAmountZ);
	/// Grows this region by the amounts specified.
	void grow(const glm::ivec3& v3dAmount);

	/// Tests whether all components of the upper corner are at least
	/// as great as the corresponding components of the lower corner.
	bool isValid() const;

	/**
	 * @return The amount of possible voxels in this region.
	 */
	int voxels() const;

	/// Moves the Region by the amount specified.
	void shift(int32_t iAmountX, int32_t iAmountY, int32_t iAmountZ);
	/// Moves the Region by the amount specified.
	void shift(const glm::ivec3& v3dAmount);
	/// Moves the lower corner of the Region by the amount specified.
	void shiftLowerCorner(int32_t x, int32_t y, int32_t z);
	/// Moves the lower corner of the Region by the amount specified.
	void shiftLowerCorner(const glm::ivec3& v3dAmount);
	/// Moves the upper corner of the Region by the amount specified.
	void shiftUpperCorner(int32_t x, int32_t y, int32_t z);
	/// Moves the upper corner of the Region by the amount specified.
	void shiftUpperCorner(const glm::ivec3& v3dAmount);

	/// Shrinks this region by the amount specified.
	void shrink(int32_t iAmount);
	/// Shrinks this region by the amounts specified.
	void shrink(int32_t iAmountX, int32_t iAmountY, int32_t iAmountZ);
	/// Shrinks this region by the amounts specified.
	void shrink(const glm::ivec3& v3dAmount);

	core::String toString() const;

private:
	int32_t m_iLowerX;
	int32_t m_iLowerY;
	int32_t m_iLowerZ;
	int32_t m_iUpperX;
	int32_t m_iUpperY;
	int32_t m_iUpperZ;
};

/**
 * @return The 'x' position of the centre.
 */
inline int32_t Region::getCentreX() const {
	return (m_iLowerX + m_iUpperX) / 2;
}

/**
 * @return The 'y' position of the centre.
 */
inline int32_t Region::getCentreY() const {
	return (m_iLowerY + m_iUpperY) / 2;
}

/**
 * @return The 'z' position of the centre.
 */
inline int32_t Region::getCentreZ() const {
	return (m_iLowerZ + m_iUpperZ) / 2;
}

inline float Region::getCentreXf() const {
	return float(m_iLowerX + m_iUpperX) / 2.0f;
}

inline float Region::getCentreYf() const {
	return float(m_iLowerY + m_iUpperY) / 2.0f;
}

inline float Region::getCentreZf() const {
	return float(m_iLowerZ + m_iUpperZ) / 2.0f;
}

/**
 * @return The 'x' position of the lower corner.
 */
inline int32_t Region::getLowerX() const {
	return m_iLowerX;
}

/**
 * @return The 'y' position of the lower corner.
 */
inline int32_t Region::getLowerY() const {
	return m_iLowerY;
}

/**
 * @return The 'z' position of the lower corner.
 */
inline int32_t Region::getLowerZ() const {
	return m_iLowerZ;
}

/**
 * @return The 'x' position of the upper corner.
 */
inline int32_t Region::getUpperX() const {
	return m_iUpperX;
}

/**
 * @return The 'y' position of the upper corner.
 */
inline int32_t Region::getUpperY() const {
	return m_iUpperY;
}

/**
 * @return The 'z' position of the upper corner.
 */
inline int32_t Region::getUpperZ() const {
	return m_iUpperZ;
}

/**
 * @return The width of the region measured in voxels.
 * @sa getWidthInCells()
 */
inline int32_t Region::getWidthInVoxels() const {
	return getWidthInCells() + 1;
}

/**
 * @return The height of the region measured in voxels.
 * @sa getHeightInCells()
 */
inline int32_t Region::getHeightInVoxels() const {
	return getHeightInCells() + 1;
}

/**
 * @return The depth of the region measured in voxels.
 * @sa getDepthInCells()
 */
inline int32_t Region::getDepthInVoxels() const {
	return getDepthInCells() + 1;
}

/**
 * @return The width of the region measured in cells.
 * @sa getWidthInVoxels()
 */
inline int32_t Region::getWidthInCells() const {
	return m_iUpperX - m_iLowerX;
}

/**
 * @return The height of the region measured in cells.
 * @sa getHeightInVoxels()
 */
inline int32_t Region::getHeightInCells() const {
	return m_iUpperY - m_iLowerY;
}

/**
 * @return The depth of the region measured in cells.
 * @sa getDepthInVoxels()
 */
inline int32_t Region::getDepthInCells() const {
	return m_iUpperZ - m_iLowerZ;
}

/**
 * @param iX The new 'x' position of the lower corner.
 */
inline void Region::setLowerX(int32_t iX) {
	m_iLowerX = iX;
}

/**
 * @param iY The new 'y' position of the lower corner.
 */
inline void Region::setLowerY(int32_t iY) {
	m_iLowerY = iY;
}

/**
 * @param iZ The new 'z' position of the lower corner.
 */
inline void Region::setLowerZ(int32_t iZ) {
	m_iLowerZ = iZ;
}

/**
 * @param iX The new 'x' position of the upper corner.
 */
inline void Region::setUpperX(int32_t iX) {
	m_iUpperX = iX;
}

/**
 * @param iY The new 'y' position of the upper corner.
 */
inline void Region::setUpperY(int32_t iY) {
	m_iUpperY = iY;
}

/**
 * @param iZ The new 'z' position of the upper corner.
 */
inline void Region::setUpperZ(int32_t iZ) {
	m_iUpperZ = iZ;
}

inline constexpr Region::Region(int mins, int maxs) :
		Region(mins, mins, mins, maxs, maxs, maxs) {
}

/**
 * Constructs a Region and clears all extents to zero.
 */
inline constexpr Region::Region() :
		Region(0, 0, 0, 0, 0, 0) {
}

/**
 * Constructs a Region and sets the extents to the specified values.
 * @param iLowerX The desired lower 'x' extent of the Region.
 * @param iLowerY The desired lower 'y' extent of the Region.
 * @param iLowerZ The desired lower 'z' extent of the Region.
 * @param iUpperX The desired upper 'x' extent of the Region.
 * @param iUpperY The desired upper 'y' extent of the Region.
 * @param iUpperZ The desired upper 'z' extent of the Region.
 */
inline constexpr Region::Region(int32_t iLowerX, int32_t iLowerY, int32_t iLowerZ, int32_t iUpperX, int32_t iUpperY, int32_t iUpperZ) :
		m_iLowerX(iLowerX), m_iLowerY(iLowerY), m_iLowerZ(iLowerZ), m_iUpperX(iUpperX), m_iUpperY(iUpperY), m_iUpperZ(iUpperZ) {
}

/**
 * Two regions are considered equal if all their extents match.
 * @param rhs The Region to compare to.
 * @return true if the Regions match.
 * @sa operator!=
 */
inline bool Region::operator==(const Region& rhs) const {
	return ((m_iLowerX == rhs.m_iLowerX) && (m_iLowerY == rhs.m_iLowerY) && (m_iLowerZ == rhs.m_iLowerZ) && (m_iUpperX == rhs.m_iUpperX) && (m_iUpperY == rhs.m_iUpperY)
			&& (m_iUpperZ == rhs.m_iUpperZ));
}

/**
 * Two regions are considered different if any of their extents differ.
 * @param rhs The Region to compare to.
 * @return true if the Regions are different.
 * @sa operator==
 */
inline bool Region::operator!=(const Region& rhs) const {
	return !(*this == rhs);
}

/**
 * The boundary value can be used to ensure a position is only considered to be inside
 * the Region if it is that far in in all directions. Also, the test is inclusive such
 * that positions lying exactly on the edge of the Region are considered to be inside it.
 * @param fX The 'x' position of the point to test.
 * @param fY The 'y' position of the point to test.
 * @param fZ The 'z' position of the point to test.
 * @param boundary The desired boundary value.
 */
inline bool Region::containsPoint(float fX, float fY, float fZ, float boundary) const {
	return (fX <= m_iUpperX - boundary) && (fY <= m_iUpperY - boundary) && (fZ <= m_iUpperZ - boundary) && (fX >= m_iLowerX + boundary) && (fY >= m_iLowerY + boundary)
			&& (fZ >= m_iLowerZ + boundary);
}

/**
 * The boundary value can be used to ensure a position is only considered to be inside
 * the Region if it is that far in in all directions. Also, the test is inclusive such
 * that positions lying exactly on the edge of the Region are considered to be inside it.
 * @param iX The 'x' position of the point to test.
 * @param iY The 'y' position of the point to test.
 * @param iZ The 'z' position of the point to test.
 * @param boundary The desired boundary value.
 */
inline bool Region::containsPoint(int32_t iX, int32_t iY, int32_t iZ, uint8_t boundary) const {
	return (iX <= m_iUpperX - boundary) && (iY <= m_iUpperY - boundary) && (iZ <= m_iUpperZ - boundary) && (iX >= m_iLowerX + boundary) && (iY >= m_iLowerY + boundary)
			&& (iZ >= m_iLowerZ + boundary);
}

/**
 * The boundary value can be used to ensure a position is only considered to be inside
 * the Region if it is that far in in the 'x' direction. Also, the test is inclusive such
 * that positions lying exactly on the edge of the Region are considered to be inside it.
 * @param pos The position to test.
 * @param boundary The desired boundary value.
 */
inline bool Region::containsPointInX(float pos, float boundary) const {
	return (pos <= m_iUpperX - boundary) && (pos >= m_iLowerX + boundary);
}

/**
 * The boundary value can be used to ensure a position is only considered to be inside
 * the Region if it is that far in in the 'x' direction. Also, the test is inclusive such
 * that positions lying exactly on the edge of the Region are considered to be inside it.
 * @param pos The position to test.
 * @param boundary The desired boundary value.
 */
inline bool Region::containsPointInX(int32_t pos, uint8_t boundary) const {
	return (pos <= m_iUpperX - boundary) && (pos >= m_iLowerX + boundary);
}

/**
 * The boundary value can be used to ensure a position is only considered to be inside
 * the Region if it is that far in in the 'y' direction. Also, the test is inclusive such
 * that positions lying exactly on the edge of the Region are considered to be inside it.
 * @param pos The position to test.
 * @param boundary The desired boundary value.
 */
inline bool Region::containsPointInY(float pos, float boundary) const {
	return (pos <= m_iUpperY - boundary) && (pos >= m_iLowerY + boundary);
}

/**
 * The boundary value can be used to ensure a position is only considered to be inside
 * the Region if it is that far in in the 'y' direction. Also, the test is inclusive such
 * that positions lying exactly on the edge of the Region are considered to be inside it.
 * @param pos The position to test.
 * @param boundary The desired boundary value.
 */
inline bool Region::containsPointInY(int32_t pos, uint8_t boundary) const {
	return (pos <= m_iUpperY - boundary) && (pos >= m_iLowerY + boundary);
}

/**
 * The boundary value can be used to ensure a position is only considered to be inside
 * the Region if it is that far in in the 'z' direction. Also, the test is inclusive such
 * that positions lying exactly on the edge of the Region are considered to be inside it.
 * @param pos The position to test.
 * @param boundary The desired boundary value.
 */
inline bool Region::containsPointInZ(float pos, float boundary) const {
	return (pos <= m_iUpperZ - boundary) && (pos >= m_iLowerZ + boundary);
}

/**
 * The boundary value can be used to ensure a position is only considered to be inside
 * the Region if it is that far in in the 'z' direction. Also, the test is inclusive such
 * that positions lying exactly on the edge of the Region are considered to be inside it.
 * @param pos The position to test.
 * @param boundary The desired boundary value.
 */
inline bool Region::containsPointInZ(int32_t pos, uint8_t boundary) const {
	return (pos <= m_iUpperZ - boundary) && (pos >= m_iLowerZ + boundary);
}

/**
 * The boundary value can be used to ensure a region is only considered to be inside
 * another Region if it is that far in in all directions. Also, the test is inclusive such
 * that a region is considered to be inside of itself.
 * @param reg The region to test.
 * @param boundary The desired boundary value.
 */
inline bool Region::containsRegion(const Region& reg, uint8_t boundary) const {
	return (reg.m_iUpperX <= m_iUpperX - boundary) && (reg.m_iUpperY <= m_iUpperY - boundary) && (reg.m_iUpperZ <= m_iUpperZ - boundary) && (reg.m_iLowerX >= m_iLowerX + boundary)
			&& (reg.m_iLowerY >= m_iLowerY + boundary) && (reg.m_iLowerZ >= m_iLowerZ + boundary);
}

inline bool Region::isValid() const {
	return m_iUpperX >= m_iLowerX && m_iUpperY >= m_iLowerY && m_iUpperZ >= m_iLowerZ;
}

/**
 * @param iAmountX The amount to move the Region by in 'x'.
 * @param iAmountY The amount to move the Region by in 'y'.
 * @param iAmountZ The amount to move the Region by in 'z'.
 */
inline void Region::shift(int32_t iAmountX, int32_t iAmountY, int32_t iAmountZ) {
	shiftLowerCorner(iAmountX, iAmountY, iAmountZ);
	shiftUpperCorner(iAmountX, iAmountY, iAmountZ);
}

/**
 * @param x The amount to move the lower corner by in 'x'.
 * @param y The amount to move the lower corner by in 'y'.
 * @param z The amount to move the lower corner by in 'z'.
 */
inline void Region::shiftLowerCorner(int32_t x, int32_t y, int32_t z) {
	m_iLowerX += x;
	m_iLowerY += y;
	m_iLowerZ += z;
}

/**
 * @param x The amount to move the upper corner by in 'x'.
 * @param y The amount to move the upper corner by in 'y'.
 * @param z The amount to move the upper corner by in 'z'.
 */
inline void Region::shiftUpperCorner(int32_t x, int32_t y, int32_t z) {
	m_iUpperX += x;
	m_iUpperY += y;
	m_iUpperZ += z;
}

/**
 * The same amount of shrinkage is applied in all directions. Negative shrinkage
 * is possible but you should prefer the grow() function for clarity.
 * @param iAmount The amount to shrink by.
 */
inline void Region::shrink(int32_t iAmount) {
	m_iLowerX += iAmount;
	m_iLowerY += iAmount;
	m_iLowerZ += iAmount;

	m_iUpperX -= iAmount;
	m_iUpperY -= iAmount;
	m_iUpperZ -= iAmount;
}

/**
 * The amount can be specified seperatly for each direction. Negative shrinkage
 * is possible but you should prefer the grow() function for clarity.
 * @param iAmountX The amount to shrink by in 'x'.
 * @param iAmountY The amount to shrink by in 'y'.
 * @param iAmountZ The amount to shrink by in 'z'.
 */
inline void Region::shrink(int32_t iAmountX, int32_t iAmountY, int32_t iAmountZ) {
	m_iLowerX += iAmountX;
	m_iLowerY += iAmountY;
	m_iLowerZ += iAmountZ;

	m_iUpperX -= iAmountX;
	m_iUpperY -= iAmountY;
	m_iUpperZ -= iAmountZ;
}

/**
 * This function only returns true if the regions are really intersecting and not simply touching.
 */
inline bool intersects(const Region& a, const Region& b) {
	// No intersection if seperated along an axis.
	if (a.getUpperX() < b.getLowerX() || a.getLowerX() > b.getUpperX())
		return false;
	if (a.getUpperY() < b.getLowerY() || a.getLowerY() > b.getUpperY())
		return false;
	if (a.getUpperZ() < b.getLowerZ() || a.getLowerZ() > b.getUpperZ())
		return false;

	// Overlapping on all axes means Regions are intersecting.
	return true;
}

extern void logRegion(const char *ctx, const voxel::Region& region);

}

#include "Box.h"

Box::Box(vec2 position, vec2 velocity, float width, float height, float mass, float orientation, float elasticity, vec4 colour)
	: Rigidbody (BOX, position, velocity, orientation, mass, elasticity)
{
	m_extents = vec2(width / 2, height / 2);
	m_colour = colour;
	m_moment = 1.0f / 12.0f * mass * width * height;
}

void Box::FixedUpdate(vec2 gravity, float timeStep)
{
	Rigidbody::FixedUpdate(gravity, timeStep);

	// Store the local axes
	float cs = cosf(m_orientation);
	float sn = sinf(m_orientation);
	m_localX = normalize(vec2(cs, sn));
	m_localY = normalize(vec2(-sn, cs));
}

void Box::Draw()
{
	// Draw using local axes
	vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;
	vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y;
	vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
	vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y;
	aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
}

// Check if any of the other box's corners are in this box
bool Box::CheckBoxCorneres(const Box& box, vec2& contact, int& numContacts, float& pen, vec2& edgeNormal)
{
	float minX, maxX, minY, maxY;
	float boxW = box.m_extents.x * 2;
	float boxH = box.m_extents.y * 2;

	int numLocalContacts = 0;
	vec2 localContact(0, 0);
	bool first = true;

	// Loop over all corners of the other box
	for (float x = -box.m_extents.x; x < boxW; x += boxW)
	{
		for (float y = -box.m_extents.y; y < boxH; y += boxH)
		{
			// Get the position in worldspace
			vec2 p = box.m_position + x * box.m_localX + y * box.m_localY;
			// Get the position in our box's space
			vec2 p0(dot(p - GetPosition(), GetLocalX()), dot(p - GetPosition(), GetLocalY()));

			// Update the extents in each cardinal direction in out box's space
			// (ie extents along the separating axes)
			if (first || p0.x < minX) minX = p0.x;
			if (first || p0.x > maxX) maxX = p0.x;
			if (first || p0.y < minY) minY = p0.y;
			if (first || p0.y > maxY) maxY = p0.y;

			// If this corner is inside the box, add it to the list of contact points
			if (p0.x >= -GetExtents().x && p0.x <= GetExtents().x &&
				p0.y >= -GetExtents().y && p0.y <= GetExtents().y)
			{
				numLocalContacts++;
				localContact += p0;
			}
			first = false;
		}
	}

	// If we lie entirely to one side of the box along one axis, we've found a serprating axis, and we can exit
	if (maxX <= GetExtents().x || minX >= GetExtents().x ||
		maxY <= GetExtents().y || minY >= GetExtents().y)
		return false;

	if (numLocalContacts == 0)
		return false;

	bool res = false;
	contact += GetPosition() + (localContact.x * GetLocalX() + localContact.y * GetLocalY()) / (float)numLocalContacts;
	numContacts++;

	// Find the minimuum penetration vector as a penetration amount and normal
	float pen0 = GetExtents().x - minX;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = GetLocalX();
		pen = pen0;
		res = true;
	}

	pen0 = GetExtents().x - maxX;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = -GetLocalX();
		pen = pen0;
		res = true;
	}

	pen0 = GetExtents().y - minY;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = GetLocalX();
		pen = pen0;
		res = true;
	}

	pen0 = GetExtents().y - maxY;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = -GetLocalX();
		pen = pen0;
		res = true;
	}
	return res;
}

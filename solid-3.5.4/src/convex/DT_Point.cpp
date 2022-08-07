/*
 * SOLID - Software Library for Interference Detection
 * 
 * Copyright (C) 2001-2003  Dtecta.  All rights reserved.
 *
 * This library may be distributed under the terms of the Q Public License
 * (QPL) as defined by Trolltech AS of Norway and appearing in the file
 * LICENSE.QPL included in the packaging of this file.
 *
 * This library may be distributed and/or modified under the terms of the
 * GNU General Public License (GPL) version 2 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.
 *
 * This library is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Commercial use or any other use of this library not covered by either 
 * the QPL or the GPL requires an additional license from Dtecta. 
 * Please contact info@dtecta.com for enquiries about the terms of commercial
 * use of this library.
 */

#include "DT_Point.h"

MT_Scalar DT_Point::supportH(const MT_Vector3& v) const
{
    return v.dot(m_point);
}

MT_Point3 DT_Point::support(const MT_Vector3& v) const
{
    return m_point;
}

bool DT_Point::ray_cast(const MT_Point3& source, const MT_Point3& target, const MT_Scalar& margin,
						 MT_Scalar& param, MT_Vector3& normal) const 
{
	MT_Vector3 r = target - source;
	MT_Scalar  delta = -source.dot(r);  
	MT_Scalar  r_length2 = r.length2();
	MT_Scalar radius = margin;
	MT_Scalar  sigma = delta * delta - r_length2 * (source.length2() - radius * radius);

	if (sigma >= MT_Scalar(0.0))
		// The line trough source and target intersects the sphere.
	{
		MT_Scalar sqrt_sigma = MT_sqrt(sigma);
		// We need only the sign of lambda2, so the division by the positive 
		// r_length2 can be left out.
		MT_Scalar lambda2 = (delta + sqrt_sigma) /* / r_length2 */ ;
		if (lambda2 >= MT_Scalar(0.0))
			// The ray points at the sphere
		{
			MT_Scalar lambda1 = (delta - sqrt_sigma) / r_length2;
			if (lambda1 <= param)
				// The ray hits the sphere, since 
				// [lambda1, lambda2] overlaps [0, param]. 
			{
				if (lambda1 > MT_Scalar(0.0))
				{
					param = lambda1;
					normal = (source + r * lambda1) / radius;
					// NB: division by m_radius to normalize the normal.
				}
				else
				{
					param = MT_Scalar(0.0);
					normal.setValue(MT_Scalar(0.0), MT_Scalar(0.0), MT_Scalar(0.0));
				}
						
				return true;
			}
		}
	}

	return false;
}




#include "clusters.h"
#include <boost/foreach.hpp>

namespace NWUClustering
{
	Clusters::~Clusters()
	{
		if(m_pts)
		{
			m_pts->m_points.clear();
			delete m_pts;
			m_pts = NULL;
		}

		if(m_kdtree)
		{
			delete m_kdtree;
			m_kdtree = NULL;
		}

        	if(m_pts_outer)
        	{
			m_pts_outer->m_prIDs.clear();
			m_pts_outer->m_ind.clear();
            		m_pts_outer->m_points.clear();
            		delete m_pts_outer;
            		m_pts_outer = NULL;
        	}

	        if(m_kdtree_outer)
        	{
	            delete m_kdtree_outer;
        	    m_kdtree_outer = NULL;
        	}
	}

	void Clusters::allocate_outer(int dims)
	{
		if(m_pts_outer == NULL)
        	{
	            	m_pts_outer = new Points_Outer;
        		m_pts_outer->m_prIDs.clear();
            		m_pts_outer->m_ind.clear();
            		m_pts_outer->m_i_dims = dims;
            		m_pts_outer->m_i_num_points = 0;
        	}
	}

	bool Clusters::addPoints(int source, int buf_size, int dims, vector<float>& raw_data)
	{
		int i, j, k, pos, num_points = buf_size / dims;

		//cout << "add points called" << endl;

		// incorrect dimension
		if(m_pts_outer->m_i_dims != dims)
			return false;

		pos = m_pts_outer->m_i_num_points;
        	m_pts_outer->m_i_num_points += num_points;
        	//m_pts_outer->m_points.resize(extents[m_pts_outer->m_i_num_points][dims]);

		//allocate memory for the points
                m_pts_outer->m_points.resize(m_pts_outer->m_i_num_points);
                for(int ll = 0; ll < m_pts_outer->m_i_num_points; ll++)
                	m_pts_outer->m_points[ll].resize(dims);

		m_pts_outer->m_prIDs.resize(m_pts_outer->m_i_num_points, -1);

		k = 0;
		for(i = 0; i < num_points; i++)
		{
			for(j = 0; j < dims; j++)
				m_pts_outer->m_points[pos][j] = raw_data[k++];

			m_pts_outer->m_prIDs[pos] = source;

			pos++;
		}

		//cout << "outer " << m_pts_outer->m_i_num_points << endl;

		return true;
	}

	bool Clusters::updatePoints(vector< vector<int> >& raw_ind)
	{
		int i, source = -1, j = -1, prev_source = -1;

		m_pts_outer->m_ind.resize(m_pts_outer->m_i_num_points, -1);

		for(i = 0; i < m_pts_outer->m_i_num_points; i++)
		{
			source = m_pts_outer->m_prIDs[i];

			if(source != prev_source)
				j = 0;

			m_pts_outer->m_ind[i] = raw_ind[source][j++];

			prev_source = source;
		}

		return true;
	}

  void Clusters::read_from_array(const float* input, int num_points, int dims)
	{
		int i, j;

		m_pts = new Points;
		m_pts->m_i_dims = dims;
		m_pts->m_i_num_points = num_points;
		m_pts->m_box = new interval[m_pts->m_i_dims];

		//allocate memory for the points
		m_pts->m_points.resize(m_pts->m_i_num_points);
		for(int ll = 0; ll < m_pts->m_i_num_points; ll++)
			m_pts->m_points[ll].resize(dims);

		const point_coord_type* pt = input;

		for (i = 0; i < num_points; i++)
		{
			for (j = 0; j < dims; j++)
			{
				m_pts->m_points[i][j] = pt[j];

				if(i == 0)
				{
					m_pts->m_box[j].upper = m_pts->m_points[i][j];
					m_pts->m_box[j].lower = m_pts->m_points[i][j];
				}
				else
				{
					if(m_pts->m_box[j].lower > m_pts->m_points[i][j])
						m_pts->m_box[j].lower = m_pts->m_points[i][j];
					else if(m_pts->m_box[j].upper < m_pts->m_points[i][j])
						m_pts->m_box[j].upper = m_pts->m_points[i][j];
				}
			}

			pt = pt + dims;
		}
	}

	int Clusters::build_kdtree()
	{
		if(m_pts == NULL)
		{
			cout << "Point set is empty" << endl;
			return -1;
		}

		//m_kdtree = new kdtree2(m_pts->m_points, true);
		m_kdtree = new kdtree2(m_pts->m_points, false);

		if(m_kdtree == NULL)
		{
			cout << "Falied to allocate new kd tree for orginial points" << endl;
			return -1;
		}

		return 0;
	}

	int Clusters::build_kdtree_outer()
	{
		if(m_pts_outer == NULL)
		{
			cout << "Outer point set is empty" << endl;
			return -1;
		}

		if(m_pts_outer->m_i_num_points > 0)
		{

			//m_kdtree_outer = new kdtree2(m_pts->m_points_outer, true);
			m_kdtree_outer = new kdtree2(m_pts_outer->m_points, false);

			if(m_kdtree_outer == NULL)
			{
				cout << "Falied to allocate new kd tree for outer points" << endl;
				return -1;
			}
		}

		return 0;
	}
}

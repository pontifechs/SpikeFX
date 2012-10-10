#include "geo/Asset.hpp"
#include "math/Vector.hpp"


#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>

Asset::Asset()
{
}

Asset::Asset(std::string filename)
{
  // Read in the .obj file into a vector
  std::string line;
  std::vector<Vector> vertices;
  vertices.push_back(Vector()); // align to 1-based .obj file format

  std::ifstream objfile (filename.c_str());
  if (objfile.is_open())
  {
    while (objfile.good())
    {
      // Grab the line
      getline(objfile,line);
      
      // Skip empty lines
      if (line == "")
      {
	continue;
      }
      
      // Grab the line type and data
      char linetype;
      float x, y, z;
      
      std::stringstream ss(line);
      ss >> linetype >> x >> y >> z;
      // TODO:: Better error checking

      Vector pt(x, y, z, 1.0);
      
      Triangle* t;

      Vector side1;
      Vector side2;
      Vector norm;
      
      switch (linetype)
      {
      case 'v':
	vertices.push_back(pt);
	break;
      case 'f':
	t = new Triangle(vertices[x],
			 vertices[y],
			 vertices[z], SOLID_DARK_RED);
	side1 = vertices[z] - vertices[x]; // vec from 1 to 3;
	side2 = vertices[y] - vertices[x]; // vec from 1 to 2
	norm = side2.cross(side1);
	norm.normalize();
	t->SetNormals(norm, norm, norm);
	
	m_geometry.push_back(t);
	break;
      }
    }
    objfile.close();
  }
}





void Asset::Draw(TexStack* override) const
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // Translate
  glTranslatef(m_trans.x(), m_trans.y(), m_trans.z());
  // Translate back
  glTranslatef(m_center.x(), m_center.y(), m_center.z());
  // Rotate
  if (m_rotation != NULL)
  {
    glMultMatrixf(m_rotation);
  }
  // Scale
  glScalef(m_scale.x(), m_scale.y(), m_scale.z());
  // Translate to Center
  glTranslatef(-m_center.x(), -m_center.y(), -m_center.z());


  for (int i = 0; i < m_geometry.size(); i++)
  {
    m_geometry[i]->Draw();
  }
  
  glPopMatrix();
}


void Asset::AddGeo(Geo* geo)
{
  if (geo != NULL)
  {
    m_geometry.push_back(geo);
  }
}

Geo* Asset::GetTransformed(Vector light, Vector origin, Vector normal)
{
  Asset* ret = new Asset();
  for (int i = 0; i < m_geometry.size(); i++)
  {
    Geo* g = m_geometry[i];
    g->SetTranslate(m_trans);
    Geo* geo_proj = g->GetTransformed(light, origin, normal);
    g->SetTranslate(Vector(0.0, 0.0, 0.0));
    if (geo_proj != NULL)
    {
      ret->AddGeo(geo_proj);
    }
  }
     
  return ret;
}

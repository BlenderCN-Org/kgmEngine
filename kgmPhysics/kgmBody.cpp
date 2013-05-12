#include "kgmBody.h"

kgmBody::kgmBody(){
 m_mass = 1.0f;
 m_friction = 0.0f;
 m_force = 0.0f; //vec3(0, 0, 0);
 m_upforce = 0.0f;
 m_velocity = 0.0f;

 m_physics = true;
 m_collision = true;
 m_gravity = true;
 m_falling = false;

 m_position = 
 m_direction = vec3(0, 0, 0);
 //m_V = m_F = m_P = vec3(0, 0, 0);

 m_shape = ShapeCylinder;

 m_extra = 0;
 m_udata = 0;
}

kgmBody::~kgmBody(){
 int i;
 for(i = 0; i < m_convex.size(); i++)
	 delete m_convex[i];
 m_convex.clear();
}

void kgmBody::collision(kgmBody* body){
}

void kgmBody::enable(bool e){
 m_physics = e;
}

void kgmBody::translate(float x, float y, float z){
 m_position = vec3(x, y, z);
}

void kgmBody::rotate(float x, float y, float z){
 m_rotation.x = x;
 m_rotation.y = y;
 m_rotation.z = z;
 m_quaternion.euler(m_rotation);
 m_direction = vec3(cos(m_rotation.z), sin(m_rotation.z), 0);
}

void kgmBody::rotate(quat& q){
 m_quaternion = q;
 mtx4 m(m_quaternion);
 m.angles(m_rotation); //m_rotation.z *= (-1.0f);
 m_direction = vec3(cos(m_rotation.z), sin(m_rotation.z), 0);
}

void kgmBody::transform(mtx4& mtr){
 mtx4 tr(m_position), rt(m_quaternion);
 //tr.translate(m_position);
 //rt.rotate(m_rotation.x, m_rotation.y, -m_rotation.z);
 mtr = rt * tr;
}

void kgmBody::setPosition(vec3& v){
 m_position = v;
 translate(v.x, v.y, v.z);
}

void kgmBody::setDirection(vec3& d){
 m_direction = d;
 m_direction.normalize();
 m_rotation.z = (float)acos(m_direction.x);
}

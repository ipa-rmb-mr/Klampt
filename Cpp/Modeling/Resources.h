#ifndef ROBOT_RESOURCES_H
#define ROBOT_RESOURCES_H

#include <KrisLibrary/utils/ResourceLibrary.h>
#include <KrisLibrary/math3d/geometry3d.h>
#include <KrisLibrary/meshing/PointCloud.h>
#include "World.h"
#include "MultiPath.h"
#include <Klampt/Contact/Stance.h>
#include <Klampt/Contact/Grasp.h>
using namespace std;

/** @file Resources.h
 * @ingroup Modeling
 * @brief Defines all Klampt Resource types, and packing / unpacking utilities.
 */

class AnyCollection;
typedef BasicResource<Config> ConfigResource;
typedef BasicResource<Vector3> Vector3Resource;
typedef BasicResource<Matrix3> Matrix3Resource;
typedef BasicResource<Matrix> MatrixResource;
typedef BasicResource<RigidTransform> RigidTransformResource;
typedef BasicResource<GeometricPrimitive3D> GeometricPrimitive3DResource;
typedef BasicResource<Camera::Viewport> ViewportResource;
//forward declarations needed before BasicResource<T> instantiation
template <> const char* BasicResourceTypeName<Config>();
template <> const char* BasicResourceTypeName<Vector3>();
template <> const char* BasicResourceTypeName<Matrix3>();
template <> const char* BasicResourceTypeName<Matrix>();
template <> const char* BasicResourceTypeName<RigidTransform>();
template <> const char* BasicResourceTypeName<Meshing::TriMesh>();
template <> const char* BasicResourceTypeName<GeometricPrimitive3D>();
template <> const char* BasicResourceTypeName<Hold>();
template <> const char* BasicResourceTypeName<Camera::Viewport>();


/** @ingroup Modeling
 * @brief Resource for multiple Config's.
 */
class ConfigsResource : public CompoundResourceBase
{
 public:
  using ResourceBase::Load;
  using ResourceBase::Save;
  virtual bool Load(istream& in);
  virtual bool Save(ostream& out);
  virtual bool Save(AnyCollection& c);
  virtual bool Load(AnyCollection& c);
  virtual const char* Type() const { return "Configs"; }
  virtual ResourceBase* Make() { return new ConfigsResource; }
  virtual ResourceBase* Copy();
  virtual vector<string> CastTypes() const;
  virtual vector<string> SubTypes() const;
  virtual ResourcePtr Cast(const char* subtype);
  virtual bool Extract(const char* subtype,vector<ResourcePtr>&);
  virtual bool Pack(vector<ResourcePtr>& subobjects,string* errorMessage=NULL);
  virtual bool Unpack(vector<ResourcePtr>& subobjects,bool* incomplete=NULL);

  vector<Vector> configs;
};

/** @ingroup Modeling
 * @brief Resource for a TriMesh.  Needs to be overloaded to load
 * from alternate mesh formats (meshing/IO.h).
 */
class TriMeshResource : public BasicResource<Meshing::TriMesh>
{
public:
  virtual bool Load(const std::string& fn);
  virtual bool Save(const std::string& fn);
  virtual const char* Type() const { return "TriMesh"; }
  virtual ResourceBase* Make() { return new TriMeshResource; }
  virtual ResourceBase* Copy();
};

/** @ingroup Modeling
 * @brief Resource for a PointCloud3D.
 */
class PointCloudResource : public ResourceBase
{
 public:
  using ResourceBase::Load;
  using ResourceBase::Save;
  virtual bool Load(istream& in);
  virtual bool Save(ostream& out);
  virtual const char* Type() const { return "PointCloud"; }
  virtual ResourceBase* Make() { return new PointCloudResource; }
  virtual ResourceBase* Copy();

  Meshing::PointCloud3D pointCloud;
};

/** @ingroup Modeling
 * @brief Resource for a Robot.
 */
class RobotResource : public ResourceBase
{
 public:
  using ResourceBase::Load;
  using ResourceBase::Save;
  virtual bool Load(const string& fn);
  virtual bool Save(const string& fn);
  virtual const char* Type() const { return "Robot"; }
  virtual ResourceBase* Make() { return new RobotResource; }
  virtual ResourceBase* Copy();

  Robot robot;
};

/** @ingroup Modeling
 * @brief Resource for a RigidObject.
 */
class RigidObjectResource : public ResourceBase
{
 public:
  using ResourceBase::Load;
  using ResourceBase::Save;
  virtual bool Load(const string& fn);
  virtual bool Save(const string& fn);
  virtual const char* Type() const { return "RigidObject"; }
  virtual ResourceBase* Make() { return new RigidObjectResource; }
  virtual ResourceBase* Copy();

  RigidObject object;
};

/** @ingroup Modeling
 * @brief Resource for a RobotWorld.
 *
 * Implementation status: Incomplete. (Saving is not done yet, visualization
 * info are not implemented in hierarchy)
 */
class WorldResource : public CompoundResourceBase
{
 public:
  using ResourceBase::Load;
  using ResourceBase::Save;
  virtual bool Load(const string& fn);
  virtual bool Save(const string& fn);
  virtual const char* Type() const { return "World"; }
  virtual ResourceBase* Make() { return new WorldResource; }
  virtual ResourceBase* Copy();
  virtual vector<string> SubTypes() const;
  virtual bool Extract(const char* subtype,vector<ResourcePtr>&);
  virtual bool Pack(vector<ResourcePtr>& subobjects,string* errorMessage=NULL);
  virtual bool Unpack(vector<ResourcePtr>& subobjects,bool* incomplete=NULL);

  RobotWorld world;
};

/** @ingroup Modeling
 * @brief Resource for a LinearPath.
 */
class LinearPathResource : public CompoundResourceBase
{
 public:
  using ResourceBase::Load;
  using ResourceBase::Save;
  virtual bool Load(istream& in);
  virtual bool Save(ostream& out);
  virtual bool Save(AnyCollection& c);
  virtual bool Load(AnyCollection& c);
  virtual const char* Type() const { return "LinearPath"; }
  virtual ResourceBase* Make() { return new LinearPathResource; }
  virtual ResourceBase* Copy();
  virtual vector<string> CastTypes() const;
  virtual vector<string> SubTypes() const;
  virtual vector<string> ExtractTypes() const;
  virtual ResourcePtr Cast(const char* subtype);
  virtual bool Extract(const char* subtype,vector<ResourcePtr>&);
  virtual bool Pack(vector<ResourcePtr>& subobjects,string* errorMessage=NULL);
  virtual bool Unpack(vector<ResourcePtr>& subobjects,bool* incomplete=NULL);

  vector<Real> times;
  vector<Vector> milestones;
};

/** @ingroup Modeling
 * @brief Resource for a MultiPath.
 * 
 * Implementation status: mostly complete. Hierarchy for properties and
 * global holds are not complete.
 */
class MultiPathResource : public CompoundResourceBase
{
 public:
  using ResourceBase::Load;
  using ResourceBase::Save;
  virtual bool Load(const string& fn);
  virtual bool Save(const string& fn);
  virtual bool Load(TiXmlElement* in);
  virtual bool Save(TiXmlElement* out);
  virtual const char* Type() const { return "MultiPath"; }
  virtual ResourceBase* Make() { return new MultiPathResource; }
  virtual ResourceBase* Copy();
  virtual vector<string> CastTypes() const;
  virtual vector<string> SubTypes() const;
  virtual vector<string> ExtractTypes() const;
  virtual ResourcePtr Cast(const char* subtype);
  virtual bool Extract(const char* subtype,vector<ResourcePtr>&);
  virtual bool Pack(vector<ResourcePtr>& subobjects,string* errorMessage=NULL);
  virtual bool Unpack(vector<ResourcePtr>& subobjects,bool* incomplete=NULL);

  MultiPath path;
};

/** @ingroup Modeling
 * @brief Resource for an IKGoal.
 * 
 * Implementation status: hierarchy is incomplete.
 */
class IKGoalResource : public CompoundResourceBase
{
 public:
  IKGoalResource() {}
  IKGoalResource(const IKGoal& val) : goal(val) {}
  IKGoalResource(const IKGoal& val,const string& name) : CompoundResourceBase(name),goal(val) {}
  using ResourceBase::Load;
  using ResourceBase::Save;
  virtual bool Load(AnyCollection& c);
  virtual bool Save(AnyCollection& c);
  virtual bool Load(std::istream& in) {
    in>>goal;
    if(in.bad()) return false;
    return true;
  }
  virtual bool Save(std::ostream& out) {
    out<<goal<<std::endl;
    return true;
  }
  virtual const char* Type() const { return "IKGoal"; }
  virtual ResourceBase* Make() { return new IKGoalResource; }
  virtual ResourceBase* Copy() { return new IKGoalResource(goal,name); }
  virtual vector<string> CastTypes() const;
  virtual ResourcePtr Cast(const char* subtype) const;

  IKGoal goal;
};

/** @ingroup Modeling
 * @brief Resource for a Hold.
 */
class HoldResource : public CompoundResourceBase
{
 public:
  HoldResource() {}
  HoldResource(const Hold& val) : hold(val) {}
  HoldResource(const Hold& val,const string& name) : CompoundResourceBase(name),hold(val) {}
  using ResourceBase::Load;
  using ResourceBase::Save;
  virtual bool Load(AnyCollection& c);
  virtual bool Save(AnyCollection& c);
  virtual bool Load(std::istream& in) {
    in>>hold;
    if(in.bad()) return false;
    return true;
  }
  virtual bool Save(std::ostream& out) {
    out<<hold<<std::endl;
    return true;
  }
  virtual const char* Type() const { return "Hold"; }
  virtual ResourceBase* Make() { return new HoldResource; }
  virtual ResourceBase* Copy() { return new HoldResource(hold,name); }
  virtual vector<string> CastTypes() const;
  virtual vector<string> SubTypes() const;
  virtual ResourcePtr Cast(const char* subtype) const;
  virtual bool Pack(vector<ResourcePtr>& subobjects,string* errorMessage=NULL);
  virtual bool Unpack(vector<ResourcePtr>& subobjects,bool* incomplete=NULL);

  Hold hold;
};

/** @ingroup Modeling
 * @brief Resource for a Stance.
 */
class StanceResource : public CompoundResourceBase
{
 public:
  StanceResource() {}
  StanceResource(const Stance& val);
  using ResourceBase::Load;
  using ResourceBase::Save;
  virtual bool Load(istream& in);
  virtual bool Save(ostream& out);
  virtual bool Load(TiXmlElement* in);
  virtual bool Save(TiXmlElement* out);
  virtual bool Load(AnyCollection& c);
  virtual bool Save(AnyCollection& c);
  virtual const char* Type() const { return "Stance"; }
  virtual ResourceBase* Make() { return new StanceResource; }
  virtual ResourceBase* Copy();
  virtual vector<string> CastTypes() const;
  virtual vector<string> SubTypes() const;
  virtual vector<string> ExtractTypes() const;
  virtual ResourcePtr Cast(const char* subtype);
  virtual bool Extract(const char* subtype,vector<ResourcePtr>&);
  virtual bool Pack(vector<ResourcePtr>& subobjects,string* errorMessage=NULL);
  virtual bool Unpack(vector<ResourcePtr>& subobjects,bool* incomplete=NULL);

  Stance stance;
};

/** @ingroup Modeling
 * @brief Resource for a Grasp.
 * 
 * Implementation status: incomplete.  Hierarchy items for fixed DOFs
 * are incomplete.
 */
class GraspResource : public CompoundResourceBase
{
 public:
  GraspResource() {}
  GraspResource(const Grasp& val);
  using ResourceBase::Load;
  using ResourceBase::Save;
  virtual bool Load(TiXmlElement* in);
  virtual bool Save(TiXmlElement* out);
  virtual bool Load(AnyCollection& c);
  virtual bool Save(AnyCollection& c);
  virtual const char* Type() const { return "Grasp"; }
  virtual ResourceBase* Make() { return new GraspResource; }
  virtual ResourceBase* Copy();
  virtual vector<string> CastTypes() const;
  virtual vector<string> SubTypes() const;
  virtual vector<string> ExtractTypes() const;
  virtual ResourcePtr Cast(const char* subtype);
  virtual bool Extract(const char* subtype,vector<ResourcePtr>&);
  virtual bool Pack(vector<ResourcePtr>& subobjects,string* errorMessage=NULL);
  virtual bool Unpack(vector<ResourcePtr>& subobjects,bool* incomplete=NULL);

  Grasp grasp;
};

/** @addtogroup Modeling */
/*@{*/

/**  @brief Initializes a ResourceLibrary so that it accepts standard RobotSim
 * file types.
 */
void MakeRobotResourceLibrary(ResourceLibrary& library);

ResourcePtr MakeResource(const string& name,const vector<int>& vals);
ResourcePtr MakeResource(const string& name,const vector<double>& vals);
ResourcePtr MakeResource(const string& name,const Config& q);
ResourcePtr MakeResource(const string& name,const vector<Config>& qs);
ResourcePtr MakeResource(const string& name,const vector<Real>& ts,const vector<Config>& qs);
ResourcePtr MakeResource(const string& name,const MultiPath& path);
ResourcePtr MakeResource(const string& name,const Vector3& pt);
ResourcePtr MakeResource(const string& name,const Matrix3& R);
ResourcePtr MakeResource(const string& name,const RigidTransform& T);
ResourcePtr MakeResource(const string& name,const GeometricPrimitive3D& geom);
ResourcePtr MakeResource(const string& name,const Meshing::TriMesh& mesh);
ResourcePtr MakeResource(const string& name,const Geometry::AnyGeometry3D& geom);
ResourcePtr MakeResource(const string& name,const IKGoal& goal);
ResourcePtr MakeResource(const string& name,const Hold& hold);
ResourcePtr MakeResource(const string& name,const Stance& stance);
ResourcePtr MakeResource(const string& name,const Grasp& grasp);

///Returns true if CastResource can cast to the given type
bool CanCastResource(const ResourcePtr& item,const char* type);
///Returns the list of types which the item is castable to
vector<string> CastResourceTypes(const ResourcePtr& item);
///Convert a resource to a given type
ResourcePtr CastResource(ResourcePtr& item,const char* type);

///Returns the list of types that can be extracted from the item
vector<string> ExtractResourceTypes(const ResourcePtr& item);
///Extract all sub-resources of a given type
vector<ResourcePtr> ExtractResources(ResourcePtr& item,const char* type);

///Creates an object of the same type as the template out of the given
///resources
ResourcePtr PackResources(vector<ResourcePtr>& resources,ResourcePtr rtemplate,string* errorMessage=NULL);
///Creates an object of the given type out of the given resources
ResourcePtr PackResources(ResourceLibrary& resources,const string& type,string* errorMessage=NULL);

///Creates sub-objects from the given resource.
///If the decomposition succeeded, the flag successful is set to true.
///If the decomposition is incomplete, the flag incomplete is set to true.
///Here "incomplete" means that calling Pack on the return array will
///NOT produce a copy of r.
vector<ResourcePtr> UnpackResource(ResourcePtr r,bool* successful=NULL,bool* incomplete=NULL);

/*@}*/

#endif

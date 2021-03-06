#ifndef IMPLICIT_DEFORMER_HPP
#define IMPLICIT_DEFORMER_HPP

#include "mesh.hpp"
#include "maya_helpers.hpp"
#include "animesh_base.hpp"

#include <maya/MPxDeformerNode.h> 

#include <memory>

class ImplicitDeformer: public MPxDeformerNode
{
public:
    static const MTypeId id;

    static void *creator() { return new ImplicitDeformer(); }
    static MStatus initialize();
    
    void postConstructor();
    MStatus connectionMade(const MPlug &plug, const MPlug &otherPlug, bool asSrc);
    MStatus connectionBroken(const MPlug &plug, const MPlug &otherPlug, bool asSrc);
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock);
    MStatus deform(MDataBlock &block, MItGeometry &iter, const MMatrix &mat, unsigned int multiIndex);
    MStatus setDependentsDirty(const MPlug &plug_, MPlugArray &plugArray);

    // Calculate the base potential based on the current mesh, and store it to the
    // basePotential attribute.
    MStatus calculate_base_potential();

    // The base potential of the mesh.
    static MObject basePotential;

    // The input implicit surface.
    static MObject implicit;

    // The number of deformer iterations to perform.
    static MObject deformerIterations;

    // Enable or disable iterative smoothing during deformation.
    static MObject iterativeSmoothing;

    // Enable or disable final fitting, which takes place between the first and second
    // pass of final smoothing.
    static MObject finalFitting;

    // The final smoothing method.  Note that this is independent of iterativeSmoothing.
    static MObject finalSmoothingMode;
    
private:
    static DagHelpers::MayaDependencies dependencies;

    void load_mesh(MDataBlock &dataBlock);
    void load_base_potential(MDataBlock &dataBlock);
    std::shared_ptr<const Skeleton> get_implicit_skeleton(MDataBlock &dataBlock);

    bool implicitIsConnected;

    // If true, the contents of basePotential have been modified and not yet loaded.
    bool basePotentialIsDirty;

    // The loaded mesh.  We own this object.
    std::unique_ptr<Mesh> mesh;

    // The main deformer implementation.
    std::unique_ptr<AnimeshBase> animesh;
};

#endif

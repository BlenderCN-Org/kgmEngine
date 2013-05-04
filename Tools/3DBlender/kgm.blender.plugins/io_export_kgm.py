#
#
#"""
#Created on Oct 3, 2010
#@author: goga
#Name: 'Kgm (.kgm)...'
#Blender: 242
#Group: 'Export'
#"""

#__author__ = "Karakal"
#__version__ = "0.1"
#__bpydoc__ = """ This script Exports Kgm format."""

bl_info = {
    "name": "Export Kgm Model Format (.kgm)",
    "author": "Karakal",
    "version": (0, 1),
    "blender": (2, 5, 3),
    "api": 31343,
    "location": "File > Export",
    "description": "Export to the Kgm Model Format (.kgm)",
    "warning": "",
    "category": "Import-Export"}

import os
import bpy
import mathutils

def toGrad(a):
  return a * 180.0 / 3.14

class kgm_panel(bpy.types.Panel):
 def draw(self, context):
  self.layout.prop(bpy.context.active_object, "object")


class kgm_actor(bpy.types.Operator):
 ''' Add kgmActor '''
 bl_idname = "object.kgm_actor_add"
 bl_label = "Add kgmActor"
 bl_options = {'REGISTER', 'UNDO'}


 def execute(self, context):
     bpy.types.Object.kgm = bpy.props.BoolProperty()
     bpy.types.Object.kgmType = bpy.props.StringProperty()
     bpy.types.Object.kgmState = bpy.props.StringProperty()
     bpy.ops.object.add()
     a = bpy.context.object
     a.name = "kgmActor"
     a.kgm = True
     a.kgmType = "None"
     a.kgmState = "None"
     return {'FINISHED'}



scene_materials = []

class kgmMaterial:
 def __init__(self, mtl):
  self.name = mtl.name
  self.diffuse = mtl.diffuse_color
  self.emmision = [1.0, 1.0, 1.0]
  self.specular = mtl.specular_color
  self.shine = 1.0
  self.alpha = mtl.alpha
  self.textures = [mtl.texture_slots[TextureSlot].texture for TextureSlot in mtl.texture_slots.keys() if mtl.texture_slots[TextureSlot].texture.type == "IMAGE"]
  #Refine a new list with only image textures that have a file source
  self.images = [os.path.basename(Texture.image.filepath) for Texture in self.textures if Texture.image.source == "FILE"]

  if mtl.name in bpy.data.materials and 'Shader' in bpy.data.materials[mtl.name]:
   self.shader = bpy.data.materials[mtl.name]['Shader']
  #if ImageFiles:
  #  return ImageFiles[0]
  pass

#light
class kgmLight:
 def __init__(self, o):
  self.name = o.name
  self.pos = mathutils.Vector((0, 0, 0)) * o.matrix_local
  self.rot = o.matrix_local.to_euler()
  lamp = o.data
  self.intensity = lamp.energy
  self.shadows = 'No' if lamp.shadow_method == 'NOSHADOW' else 'Yes'
  self.range = 2 * lamp.distance
  self.color = lamp.color
  self.type = 'Point' if lamp.type == 'POINT' else 'Spot'

#camera
class kgmCamera:
 def __init__(self, o):
  self.name = o.name
  self.pos = mathutils.Vector((0, 0, 0)) * o.matrix_local
  self.rot = o.matrix_local.to_euler()
  self.angle = o.data.angle
  self.near = o.data.clip_start
  self.far = o.data.clip_end

#skeleton
class kgmBone:
 def __init__(self, bone, gmtx):
  self.parent = bone.parent.name if bone.parent else 'None'
  self.name = bone.name
  self.mtx = gmtx * bone.matrix_local
  #self.pos = mathutils.Vector((0, 0, 0)) * bone.matrix_local
  par_pos = bone.parent.matrix_local * mathutils.Vector((0, 0, 0)) if bone.parent else mathutils.Vector((0, 0, 0))
  par_quat = bone.parent.matrix_local.to_quaternion() if bone.parent else mathutils.Quaternion((0, 0, 0, 1))
  loc_pos = bone.matrix_local * mathutils.Vector((0, 0, 0))

  self.pos = self.mtx.to_translation()
  self.quat = self.mtx.to_quaternion()
  self.euler = self.mtx.to_euler('XYZ')
  #self.pos = bone.matrix_local * mathutils.Vector((0, 0, 0)) - par_pos
  #self.quat = bone.matrix_local.to_quaternion() - par_quat
  #self.euler = bone.matrix_local.to_euler('XYZ')
  print(bone.name + " " + str(par_pos) + " " + str(loc_pos))

class kgmSkeleton:
 def __init__(self, o):
  self.name = o.name
  self.bones = []

  #if bpy.ops.object.mode_set.poll():
  bpy.ops.object.mode_set(mode='POSE')

  armature = o.data
  self.mtx = o.matrix_local
  self.pos = o.matrix_local.to_translation() # * mathutils.Vector((0, 0, 0))
  self.quat = o.matrix_local.to_quaternion()
  self.euler = o.matrix_local.to_euler('XYZ')
  list = [bone for bone in armature.bones if bone.parent == None]
  print(o.name)
  print(list)
  for o in list:
   self.collect(o)

  #if bpy.ops.object.mode_set.poll():
  bpy.ops.object.mode_set(mode='OBJECT')


 def collect(self, o):
  self.bones.append(kgmBone(o, self.mtx))
  for bone in o.children:
   self.collect(bone)



class kgmVertex:
 def __init__(self):
  self.v = [0, 0, 0]
  self.n = [0, 0, 0]
  self.uv = [0, 0]
  self.ib = [-1, -1, -1, -1]
  self.wb = [0, 0, 0, 0]
  self.idx = -1

class kgmFace:
 def __init__(self, a, b, c):
  self.f = [a, b, c]


class kgmMesh:
 def __init__(self, o):
#  mesh = o.to_mesh(bpy.context.scene, False, "PREVIEW")
  mesh = o.data
  mtx = o.matrix_local

  print('Current mesh: ' + mesh.name)
  self.name = mesh.name
  self.vertices = []
  self.faces = []
  self.skin = False
  self.mtls = []
  self.hasvgroups = True if len(o.vertex_groups) > 0 else False
  print('VertexGroups: ' + str(len(o.vertex_groups)))

  for m in mesh.materials:
   b = False
   self.mtls.append(m.name)

   for n in scene_materials:
    if n.name == m.name:
     b = True
   if b == False:
    scene_materials.append(kgmMaterial(m))

  uvcoord = len(mesh.uv_textures)

  if hasattr(mesh, 'faces'):
    print('Faces: ' + str(len(mesh.faces)))
    for i in range(0, len(mesh.faces)):
     face = mesh.faces[i]
     iface = [-1, -1, -1, -1]
     uvface = mesh.uv_textures.active.data[i] if uvcoord else None

     for j in range(0, len(face.vertices)):
      v = kgmVertex();
      vi = face.vertices[j]
      c = mesh.vertices[vi].co * mtx
      n = face.normal * mtx.to_3x3()
      v.v = [c[0], c[1], c[2]]
      v.n = [n[0], n[1], n[2]]

      if self.hasvgroups == True:
       for g in range(0, len(mesh.vertices[vi].groups)):
        if g < 4:
         v.ib[g] = mesh.vertices[vi].groups[g].group
         v.wb[g] = mesh.vertices[vi].groups[g].weight

      if uvface:
       uv = uvface.uv[j]
       v.uv = [uv[0], uv[1]]

      v.idx = vi
      iface[j] = self.addVertex(v)

     if len(face.vertices) == 4:
      self.faces.append(kgmFace(iface[0], iface[1], iface[2]))
      self.faces.append(kgmFace(iface[0], iface[2], iface[3]))
     else:
      self.faces.append(kgmFace(iface[0], iface[1], iface[2]))

 def addVertex(self, vx):
  iv = -1
  #check if such vertex already exist
  for i in range(0, len(self.vertices)):
   v = self.vertices[i]
   if (v.v[0] == vx.v[0]) and (v.v[1] == vx.v[1]) and (v.v[2] == vx.v[2]):
    if (v.uv[0] == vx.uv[0]) and (v.uv[1] == vx.uv[1]):
     iv = i
     if (v.n[0] != vx.n[0]) or (v.n[1] != vx.n[1]) or (v.n[2] != vx.n[2]):
      n1 = mathutils.Vector((v.n[0], v.n[1], v.n[2]))
      n2 = mathutils.Vector((vx.n[0], vx.n[1], vx.n[2]))
      n = n1 + n2
      n.normalize()
      v.n = [n.x, n.y, n.z]
     break

  if iv < 0:
   self.vertices.append(vx)
   iv = (len(self.vertices) - 1)
  return iv

 def skinVertex(self, o, m, v):
  armatures = [modifier for modifier in o.modifiers if modifier.type == 'ARMATURE']
  if armatures:
   armature = armatures[0].object
   bones = armature.data.bones

 def skin(self, o, m):
  armatures = [modifier for modifier in o.modifiers if modifier.type == 'ARMATURE']
  if armatures:
   armature = armatures[0].object
   bones = armature.data.bones
   poseBones = armature.pose.bones
   maxInfluences = 0
   usedBones = set()
   vertexGroups = {}

   for v in m.vertices:
    boneInfluences = [poseBones[o.vertex_groups[group.group].name] for group in v.goups if o.vertex_groups[group.group].name in poseBones]
    if len(boneInfluences) > maxInfuences:
     maxInfluennces = len(boneInfluences)
    for bone in boneInfluences:
     usedBones.add(bone)

class kgmFrame:
 def __init__(self, t, x, y, z, rx, ry, rz, rw):
  self.key = t
  self.pos = [x, y, z]
  self.rot = [rx, ry, rz, rw]

class kgmAnimation:
 def __init__(self, o):
  self.name = o.name
  self.type = o.type
  self.frames = []
  frameCount = bpy.context.scene.frame_end - bpy.context.scene.frame_start + 1

  for frame in range(0, frameCount):
   bpy.context.scene.frame_set(frame)
   pos = o.matrix_world.to_translation()
   rot = o.matrix_world.to_quaternion()
   f = kgmFrame(frame, pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, rot.w)
   self.frames.append(f)


class kgmBoneAnimation(kgmAnimation):
 def __init__(self, o, a):
  self.name = o.name
  self.type = 'BONE'
  self.frames = []

  currentScene = bpy.context.scene
  startFrame = currentScene.frame_start
  endFrame = currentScene.frame_end

  for frame in range(startFrame, endFrame+1, 1):
   currentScene.frame_current = frame
   currentScene.frame_set(frame)
   allBones = currentScene.objects[a.name].pose.bones
   for bn in allBones:
    if bn.name == o.name:
     #print("Bone " + " name: " + bn.name + " frame: " + str(frame) +  " mtx: " + str(bn.matrix))
     mtx = bn.matrix.to_4x4()
     pos = mtx.to_translation()
     rot = mtx.to_quaternion()
     f = kgmFrame(frame, pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, rot.w)
     self.frames.append(f)
  currentScene.frame_set(startFrame)


class kgmActor:
 def __init__(self, o):
  self.name = o.name
  self.type = o.kgmType
  self.state = o.kgmState
  self.mtx = o.matrix_world
  self.pos = mathutils.Vector((0, 0, 0)) * self.mtx
  self.rot = self.mtx.to_euler()
  self.quat = self.mtx.to_quaternion()

import threading
class kgmThread(threading.Thread):
  def __init__(self, c):
    self.context = c

  def run(self):
    pass

from bpy.props import *
class kgmExport(bpy.types.Operator):
 '''This appiers in the tooltip of '''
 bl_idname = "export_scene.kgm" # this is important since its how bpy.ops.export.kgm_export() is constructed
 bl_label = "Export Kgm"

 # List of operator properties, the attributes will be assigned
 # to the class instance from the operator settings before calling.

   # TODO, add better example props
 filepath = StringProperty(name="File Path", description="File path used for exporting the Kgm file", maxlen=1024, default="~/")
   #use_setting = BoolProperty(name="Example Boolean", description="Example Tooltip", default= True)
 exp_meshes = BoolProperty(name="Export Meshes", description="", default=True)
 exp_lights = BoolProperty(name="Export Lights", description="", default=True)
 exp_materials = BoolProperty(name="Export Materials", description="", default=True)
 exp_cameras = BoolProperty(name="Export Cameras", description="", default=False)
 exp_armatures = BoolProperty(name="Export Armatures", description="", default=False)
 exp_animations = BoolProperty(name="Export Animations", description="", default=False)
 exp_kgmactors = BoolProperty(name="Export kgmActors", description="", default=True)
 is_selection = BoolProperty(name="Selected only", description="", default=False)
 type = bpy.props.EnumProperty(items=(('OPT_A', "Xml", "Xml format"), ('OPT_B', "Bin", "Binary format")), name="Format", description="Choose between two items", default='OPT_A')


 @classmethod
 def poll(cls, context):
  return context.active_object != None

 def execute(self, context):
 #Bug, currently isnt working
 #if not self.is_property_set("filepath"):
 #raise Exception("filename not set")

  scene = context.scene
  for o in scene.objects:
   print(o.name + o.type)

  if self.is_selection:
   objects = [ob for ob in scene.objects if ob.is_visible(scene) and ob.select]
  else:
   objects = [ob for ob in scene.objects if ob.is_visible(scene)]

  #cFrame = bpy.context.scene.frame_current

  print("Collect Objects...")
  meshes = [kgmMesh(ob) for ob in objects if ob.type == 'MESH' and self.exp_meshes]
  materials = [ob for ob in scene_materials if self.exp_materials]
  lights = [kgmLight(ob) for ob in objects if ob.type == 'LAMP' and self.exp_lights]
  cameras = [kgmCamera(ob) for ob in objects if ob.type == 'CAMERA' and self.exp_cameras]
  skeletons = [kgmSkeleton(ob) for ob in objects if ob.type == 'ARMATURE' and self.exp_armatures]
  actors = [kgmActor(ob) for ob in objects if ob.type == 'EMPTY' and self.exp_kgmactors and hasattr(ob, 'kgm')]
#  animations = [kgmAnimation(ob) for ob in objects if self.exp_animations]
  animations = []
  
  if self.exp_animations:
    armatures = [ob for ob in objects if ob.type == 'ARMATURE']
    for armature in armatures:
      print("scan armature")
      for bone in armature.data.bones:
        animations.append(kgmBoneAnimation(bone, armature))

  #bpy.context.scene.frame_current = cFrame

  print("Animations: " + str(len(animations)))
  print("Objects: " + str(len(objects)))
  print("Mehses: " + str(len(meshes)))
  print("Lights: " + str(len(lights)))

#  path = self.filepath
  if not self.filepath.lower().endswith(".kgm"):
   self.filepath += ".kgm"

  file = open(self.filepath, "w")
  file.write("<?xml version='1.0'?>\n")

  #scene data
  frames = bpy.context.scene.frame_end - bpy.context.scene.frame_start + 1
  fstart = bpy.context.scene.frame_start
  fend = bpy.context.scene.frame_end
  file.write("<kgm>\n")

  #animations
  if self.exp_animations:
   file.write(" <kgmAnimation fstart='" + str(fstart) + "' fend='" + str(fend) + "'>\n")
   for o in animations:
    file.write("  <Animation name='" + o.name + "'>\n")
    for f in o.frames:
     file.write("   <Frame key='" + str(f.key) + "' position='" + str(f.pos[0]) + " " + str(f.pos[1]) + " " + str(f.pos[2]) + "' quaternion='" + str(f.rot[0]) + " " + str(f.rot[1]) + " " + str(f.rot[2]) + " " + str(f.rot[3]) + "' />\n")
    file.write("  </Animation>\n")
   file.write(" </kgmAnimation>\n")

  #materials
  for o in scene_materials:
   file.write(" <kgmMaterial name='" + o.name + "'>\n")
   file.write("  <Color value='" + str(o.diffuse[0]) + " " + str(o.diffuse[1]) + " " + str(o.diffuse[2]) + "'/>\n")
   file.write("  <Emmision value='" + str(o.emmision[0]) + " " + str(o.emmision[1]) + " " + str(o.emmision[2]) + "'/>\n")
   file.write("  <Specular value='" + str(o.specular[0]) + " " + str(o.specular[1]) + " " + str(o.specular[2]) + "'/>\n")
   file.write("  <Shininess value='" + str(o.shine) + "'/>\n")
   file.write("  <Alpha value='" + str(o.alpha) + "'/>\n")
   if(o.images):
    file.write("  <Texture value='" + o.images[0] + "'/>\n")
   if(hasattr(o, 'shader')):
    file.write("  <Shader value='" + o.shader + "'/>\n")
   file.write(" </kgmMaterial>\n")

  #lights
  for o in lights:
   file.write(" <kgmLight name='" + o.name + "'>\n")
   file.write("  <Type value='" + o.type + "'/>\n")
   file.write("  <Color value='" + str(o.color[0]) + " " + str(o.color[1]) + " " + str(o.color[2]) + "'/>\n")
   file.write("  <Position value='" + str(o.pos[0]) + " " + str(o.pos[1]) + " " + str(o.pos[2]) + "'/>\n")
   file.write("  <Rotation value='" + str(o.rot[0]) + " " + str(o.rot[1]) + " " + str(o.rot[2]) + "'/>\n")
   file.write("  <Intensity value='" + str(o.intensity) + "'/>\n")
   file.write("  <Shadows value='" + str(o.shadows) + "'/>\n")
   file.write(" </kgmLight>\n")

  #cameras
  for o in cameras:
   file.write(" <kgmCamera name='" + o.name + "'>\n")
   file.write("  <Position value='" + str(o.pos[0]) + " " + str(o.pos[1]) + " " + str(o.pos[2]) + "'/>\n")
   file.write("  <Rotation value='" + str(o.rot[0]) + " " + str(o.rot[1]) + " " + str(o.rot[2]) + "'/>\n")
   file.write("  <Clip angle='" + str(o.angle) + "' zfar='" + str(o.far) + "' znear='" + str(o.near) + "'/>\n")
   file.write(" </kgmCamera>\n")

  #meshes
  for o in meshes:
   file.write(" <kgmMesh name='" + o.name + "'>\n")
   if len(o.mtls) > 0:
    file.write("  <Material name='" + o.mtls[0] + "' />\n")
     
   file.write("  <Vertices length='" + str(len(o.vertices)) + "'>\n")
   for v in o.vertices:
    file.write("   " + str(v.v[0]) + " " + str(v.v[1]) + " " + str(v.v[2]))
    file.write(" " + str(v.n[0]) + " " + str(v.n[1]) + " " + str(v.n[2]))
    file.write(" " + str(v.uv[0]) + " " + str(v.uv[1]))
    file.write("\n")
   file.write("  </Vertices>\n")

   file.write("  <Faces length='" + str(len(o.faces)) + "'>\n")
   for f in o.faces:
    file.write("   " + str(f.f[0]) + " " + str(f.f[1]) + " " + str(f.f[2]) + "\n")
   file.write("  </Faces>\n")

   if o.hasvgroups == True:
    file.write("  <Skin length='" + str(len(o.vertices)) + "'>\n")
    for v in o.vertices:
     file.write("   " + str(v.ib[0]) + " " + str(v.ib[1]) + " " + str(v.ib[2]) + " " + str(v.ib[3]))
     file.write(" " + str(v.wb[0]) + " " + str(v.wb[1]) + " " + str(v.wb[2]) + " " + str(v.wb[3]))
     file.write("\n")
    file.write("  </Skin>\n")
   file.write(" </kgmMesh>\n")

  #skeletons
  for s in skeletons:
   file.write(" <kgmSkeleton name='" + s.name +
              "' position='" + str(s.pos.x) + " " + str(s.pos.y) + " " + str(s.pos.z) +
              "' quaternion='" + str(s.quat.x) + " " + str(s.quat.y) + " " + str(s.quat.z) + " " + str(s.quat.w) +
#              "' euler='" + str(toGrad(s.euler.x)) + " " + str(toGrad(s.euler.y)) + " " + str(toGrad(s.euler.z)) +
              "'>\n")
   for b in s.bones:
    file.write("  <Bone name='" + b.name + "' parent='" + b.parent + "'")
    file.write(" position='" + str(b.pos.x) + " " + str(b.pos.y) + " " + str(b.pos.z) + "'")
    #file.write(" rotation='" + str(b.rot.x) + " " + str(b.rot.y) + " " + str(b.rot.z) + "'")
    file.write(" quaternion='" + str(b.quat.x) + " " + str(b.quat.y) + " " + str(b.quat.z) + " " + str(b.quat.w) + "'")
#    file.write(" euler='" + str(toGrad(b.euler.x)) + " " + str(toGrad(b.euler.y)) + " " + str(toGrad(b.euler.z)) + "'")
    file.write("/>\n")
   file.write(" </kgmSkeleton>\n")

  #skeletons
  for a in actors:
   file.write(" <kgmActor name='" + a.name + "' type='" + a.type + "'>\n")
   file.write("  <Position value='" + str(a.pos[0]) + " " + str(a.pos[1]) + " " + str(a.pos[2]) + "'/>\n")
   file.write("  <Rotation value='" + str(a.rot[0]) + " " + str(a.rot[1]) + " " + str(a.rot[2]) + "'/>\n")
   file.write("  <State value='" + a.state + "'/>\n")
   file.write(" </kgmActor>\n")

  file.write("</kgm>\n")
  file.close()
  return {'FINISHED'}

 def invoke(self, context, event):
  wm = context.window_manager

  if True:
   # File selector
   wm.fileselect_add(self) # will run self.execute()
   return {'RUNNING_MODAL'}
  elif True:
   # search the enum
   wm.invoke_search_popup(self)
   return {'RUNNING_MODAL'}
  elif False:
   # Redo popup
   return wm.invoke_props_popup(self, event) #
  elif False:
   return self.execute(context)


#---------------------------
def menu_func(self, context):
    self.layout.operator(kgmExport.bl_idname, text="Karakal game (.kgm)", icon='PLUGIN')
    #default_path = os.path.splitext(bpy.data.filepath)[0] + ".kgm"
    #self.layout.operator(kgmExport.bl_idname, text="Kgm (.kgm)").filepath = default_path

def menu_func_a(self, context):
    self.layout.operator(kgm_actor.bl_idname, text="kgmActor", icon='PLUGIN')

def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func)
    bpy.types.INFO_MT_add.append(menu_func_a)

def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
    register()

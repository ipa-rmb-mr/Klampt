I/O
===================

Klamp't supports many types of I/O formats:

-  Custom file formats (old-style): Klamp't defines several formats for
   its custom types. Compared to the JSON custom formats, these are
   older but are more compatible with C++ backend, especially the
   RobotPose app.
-  Custom file formats (JSON): We are beginning to use a newer JSON
   format that will be supported more heavily in the future.
-  URDF files: Klamp't natively supports ROS' Universal Robot
   Description Format.
-  Geometry files: Klamp't natively supports OFF (Object File Format),
   OBJ (Wavefront OBJ), and PCD (Point Cloud Data) file formats. If it
   is built with Assimp support, then any file format that Assimp reads
   can also be read as a mesh.
-  ROS: Klamp't can publish and subscribe to several ROS message types.
-  Three.js: Klamp't can export Three.js scenes.
-  MPEG: Klamp't's SimTest app and Python vis module can export MPEG
   videos of simulations, if ffmpeg is installed.
-  HTML: Klamp't's Python vis module can export HTML files for
   simulation playback.

Custom file formats in Klamp't
------------------------------

+-------------------------------+-------------+
| Object type                   | Extension   |
+===============================+=============+
| World model                   | .xml        |
+-------------------------------+-------------+
| Robot model                   | .rob        |
+-------------------------------+-------------+
| Rigid object model            | .obj        |
+-------------------------------+-------------+
| Terrain model                 | .env        |
+-------------------------------+-------------+
| Point                         | .vector3    |
+-------------------------------+-------------+
| Rigid transform               | .xform      |
+-------------------------------+-------------+
| Geometric primitive           | .geom       |
+-------------------------------+-------------+
| Configuration                 | .config     |
+-------------------------------+-------------+
| Configuration list            | .configs    |
+-------------------------------+-------------+
| IK Objective                  | .ikgoal     |
+-------------------------------+-------------+
| Piecewise linear trajectory   | .path       |
+-------------------------------+-------------+
| Multipath                     | .xml        |
+-------------------------------+-------------+
| Hold                          | .hold       |
+-------------------------------+-------------+
| Stance                        | .stance     |
+-------------------------------+-------------+

These are described in more detail in the `File Types
section <Manual-FileTypes.html>`__.


API summary
--------------------

To load/save general objects, there are are two modules to use:
`io.loader <klampt.io.html#module-klampt.io.loader>`__
and
`io.resource <klampt.io.html#module-klampt.io.resource>`__.
The resource module is nice to use because it performs automatic
identification of types, allows file browsing, and will launch a visual
editor if an object does not exist.

As an example with an IK Objective:

.. code:: python

    obj = ik.objective(...) #set up objective

    #method 1: using the resource module
    from klampt.io import resource
    resource.set("test_objective.ikgoal",obj)     # Saves obj to disk in a file compatible with robotPose
    obj2 = resource.get("test_objective.ikgoal")  # Loads the same item from disk 
    #Note: can also use .json extension, and this will automatically be converted to JSON
    #resource.set("test_objective.json",obj) 
    #obj2 = resource.get("test_objective.json") 
    #Note: can also launch a file browser
    #resource.save(obj,[optional directory])
    #resource.load("IKObjective",[optional directory])

    #method 2: serializing and deserializing in the old format
    from klampt.io import loader
    s = loader.writeIKObjective(obj)  #converts to a string representation compatible with RobotPose
    print s
    obj2 = loader.readIKObjective(s)  #reads from a string compatible with RobotPose

    #method 3: saving and loading in the old format
    loader.save(obj,"IKObjective","test_ik_objective.ikgoal")  #saves to a file on disk compatible with RobotPose
    obj2 = loader.load("IKObjective","test_ik_objective.ikgoal")  #reads from the file on disk compatible with RobotPose

    #method 4: serializing and deserializing in the JSON format
    import json
    jsonobj = loader.toJson(obj)  #converts to a data structure compatible with JSON I/O routines
    s = json.dumps(jsonobj) #converts to a JSON string
    print s
    jsonobj2 = json.parse(s)  #converts from a JSON string
    obj2 = loader.fromJson(jsonobj2)  #converts from a JSON-compatible data structure

    #method 5: saving and loading in the JSON format
    jsonobj = loader.toJson(obj)  #converts to a data structure compatible with JSON I/O routines
    with open("test_ik_objective.json",'w') as f:
        json.dump(f,jsonobj)  #saves to a JSON file on disk
    with open("test_ik_objective.json",'r') as f:
        jsonobj2 = json.load(f)
    obj2 = loader.fromJson(jsonobj2)  #converts from a JSON-compatible data structure

World loading and saving
------------------------

To load items into a :class:`~klampt.WorldModel`, use the ``readFile``
and ``loadElement`` methods. For the most part, these will automatically
figure out the type of the loaded object.  For more control, you can call
``world.makeRobot/makeRigidObject/makeTerrain(name)`` and then call
``element.loadFile(fn)`` on the resulting element.

.. note::
   Geometry files are converted into static terrains.  To make a geometry file
   into a rigid object, you will need to create a Rigid Object ``.obj`` file
   or use shim code like::

      obj.world.makeRigidObject("myobj")
      obj.geometry().loadFile("geometry.stl")
      #... if you are doing simulation, need to set up the mass
      #    properties here...

To save a ``WorldModel``, you can use the ``writeFile(fn)`` method. This will dump
all elements contained in the world into a folder of the same name as
``fn``, but without the .xml extension. Here, the paths of geometry
files will be preserved, unless the geometry has been modified.

To save individual elements (robots, objects, or terrains), you can use the
``element.saveFile(fn)`` method.

Robot (.rob and .urdf) loading and saving
-----------------------------------------

Robots are loaded from Klamp't-specific ``.rob`` files or more widely-used
URDF files. These are simple text files that are editable by hand.


Although URDF is more commonly used in the robotics field, there are
some convenient aspects of ``.rob`` files that may be useful. For example,
the ``mount`` command allows robot grippers and other attachments to
be added automatically at load-time. This is annoying to do with URDF,
requiring a separate command line step with the xacro tool.

The basic URDF file format does not specify some aspects of Klamp't
robots. These can be added under the ``<klampt>`` XML tag. See the `file
format documentation <Manual-FileTypes.html>`__ or the Klampt `import
robot tutorial <https://github.com/krishauser/Klampt/blob/master/Cpp/docs/Tutorials/Import-and-calibrate-urdf.md>`__
for more details.

For simulation purposes, Klamp't will need some motor parameters to be
tweaked (``servoP``, ``servoI``, ``servoD``, ``dryFriction``,
``viscousFriction``). This can be done by hand by tuning and
"exercising" the robot in simulation. The Driver window in SimTest can
be used for this purpose. An automatic solution is given by the
``MotorCalibrate`` program, which will optimize the constants to match a
dataset of sensed and commanded joint angles that you record while
exercising the physical robot. See `the apps
documentation <https://github.com/krishauser/Klampt/blob/master/Cpp/docs/Manual-Apps.md#motorcalibrate>`__ for more details.

The `URDFtoRob <http://github.com/krishauser/Klampt/blob/master/Cpp/docs/Manual-Apps.md#urdftorob>`__ program converts from .urdf to
.rob files. Geometric primitive link geometries will be converted to
triangle meshes.

ROS Communication
-----------------

If you build from source with ROS installed on your system, Klamp't will support
many ROS types, including Pose, PoseStamped, WrenchStamped,
Float32MultiArray, JointState, PointCloud2, Image, CameraInfo, and
JointTrajectory.  However, at the moment, the Python interface only
provides a method to subscribe to point clouds.  Support for other
messages is planned in the future.

Using :class:`~klampt.Geometry3D`, you can easily subscribe to a ROS topic containing
``PointCloud2`` messages.
This is accomplished via the :meth:`~klampt.io.SubscribeToStream` method, which
takes as arguments the protocol (currently only "ros" protocol is
supported) and the name of the ROS topic to subscribe to. For an
example, create a new file called "pointCloudFromROS.py" and copy the
following lines:

.. code:: python

    import time
    from klampt import io,PointCloud, Geometry3D, Appearance

    #Create point cloud subscriber
    topic = "myROSTopic"  # ROS topic containing point cloud, change this
                          # to whatever your publisher is publishing to
    g = Geometry3D(PointCloud())    # make 3d geometry of type PointCloud

    #Subscribe to topic
    if io.SubscribeToStream(g,"ros",topic):       #subscribe to myROSTopic
        print "Subscribed!"
    else:
        print "Could not subscribe to", topic

    numReceived = 0
    t0 = time.time()
    while True:
        processed = io.ProcessStreams()
        if processed:
            print "Received a PointCloud on",topic,"with",g.getPointCloud().numPoints(),"points"
            numReceived += 1
        time.sleep(0.01)
    
    #Unsubscribe from topic -- not strictly necessary
    io.DetachFromStream("ros",topic)    

Now run the script via

::

    python pointCloudFromROS.py

Congratulations, you have subscribed to your first point cloud!

.. note::
    If you are drawing a point cloud that will continuously update, the visualization
    may not update when the geometry does, because it will use cached values for the
    appearance.  To get the appearance to recognize the update, call:

    .. code:: python

        Appearance.refresh()

    If you are using the vis scene manager, you can also do:

    .. code:: python
    
        vis.dirty(path_to_geometry)

For additional examples, see ``Klampt-examples/Python/demos/ros_point_cloud_show.py``


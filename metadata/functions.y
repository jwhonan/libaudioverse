functions:
  Lav_initialize:
    category: core
    doc_description: |
      This function initializes Libaudioverse.
      You must call it before calling any other functions.
  Lav_shutdown:
    category: core
    doc_description: |
      Shuts down Libaudioverse.
      You must call this function at the end of your application.
      Failure to do so may cause crashes as Libaudioverse may attempt to make use of resources you passed it as your app shuts down,
      for example callbacks.
      
      Once this function has been called, all pointers and handles from Libaudioverse are invalid.
      Continuing to make use of any resource Libaudioverse has given you after this point will cause crashing.
  Lav_isInitialized:
    category: core
    doc_description: |
      Indicates whether Libaudioverse is initialized.
    params:
      destination: After a call to this function, contains 1 if Libaudioverse is initialized and 0 if it is not.
  Lav_free:
    category: core
    doc_description: |
      Frees pointers that Libaudioverse gives  you.
      There are a limited number of cases wherein Libaudioverse will return a newly allocated pointer.
      Such cases should be clearly documented in this manual.
      In order to free such memory, be sure to use this function rather than the normal system free:
      on some platforms, the Libaudioverse DLL may not be using the same C runtime,
      and the memory passed to you may be allocated from internal caches.
    params:
      ptr: The pointer to free.
  Lav_handleIncRef:
    category: core
    doc_description: |
      Newly allocated Libaudioverse handles have a reference count of 1.
      This function allows incrementing this reference count.
      If you are working in C, this function is not very helpful.
      It is used primarily by the various programming language bindings
      in order to make the garbage collector play nice.
    params:
      handle: The handle whose reference count is to be incremented.
  Lav_handleDecRef:
    category: core
    doc_description: |
      Decrement the reference count of a Libaudioverse handle.
      This function is the equivalent to Lav_free for objects.
      Note that this is only a decrement.
      if you call it in the middle of a block or in a variety of other situations, you may see the same handle again via a callback.
      
      It is not possible for Libaudioverse to provide a guaranteed freeing function.
      Such a function would have to block, making it unusable in garbage collected languages.
      Furthermore, in the case of objects like buffers, various internal Libaudioverse properties could conceivably need to hold onto the object.
      Internally, Libaudioverse uses shared pointers to make sure this cannot happen,
      but at the cost of not being able to guarantee instant resource freeing.
    params:
      handle: The handle whose reference count we are decrementing.
  Lav_handleGetAndClearFirstAccess:
    category: core
    doc_description: |
      Checks the handle's first access flag and clears it.
      This is an atomic operation, used by bindings to automatically increment and decrement handle reference counts appropriately.
      Namely, in the case of this function indicating that the first access flag is set, we avoid incrementing the reference count as it is 1 but we have no other external copies of the handle which will be decremented.
    params:
      handle: The handle to check.
      destination: 1 if the first access flag is set, otherwise 0.
  Lav_handleGetRefCount:
    category: core
    doc_description: |
      For debugging.  Allows obtaining the current reference count of the handle.
    params:
      handle: The handle to obtain the reference count of
      destination: After a call to this function, contains the reference count of the handle.
  Lav_handleGetType:
    category: core
    doc_description: |
      Returns the type of the handle.
    params:
      handle: The handle to obtain the type of.
      destination: A Lav_OBJTYPE_* constant corresponding to the handle's type.
  Lav_setLoggingCallback:
    category: core
    doc_description: |
      Configure a callback to receive logging messages.
      Note that this function can be called before Libaudioverse initialization.
      
      The callback will receive 3 parameters: level, message, and is_final.
      Level is the logging level.
      Message is the message to log.
      is_final is always 0, save when the message is the last message the logging callback will receive, ever.
      Use is_final to determine when to deinitialize your Libaudioverse logging.
    params:
      cb: The callback to use for logging.
  Lav_getLoggingCallback:
    category: core
    doc_description: |
      Get the logging callback.
    params:
      destination: The pointer to the logging callback if set, otherwise NULL.
  Lav_setLoggingLevel:
    category: core
    doc_description: |
      Set the logging level.
      You will receive messages via the logging callback for all levels less than or equal to the logging level.
    params:
      level: The new logging level.
  Lav_getLoggingLevel:
    category: core
    doc_description: |
      Get the current logging level
    params:
      destination: Contains the current logging level.
  Lav_setHandleDestroyedCallback:
    category: core
    doc_description: |
      Set the callback to be called when a Libaudioverse handle is permanently destroyed.
      Libaudioverse handles cannot be reused.
      When this callback is called, it is the last time your program can see the specific handle in question,
      and further use of that handle will cause crashes.
      
      This exists primarily for language bindings.
      If there is a case in which your C app must know if a handle is still valid, you may have design issues.
    params:
      cb: The callback to be called when handles are destroyed.
  Lav_deviceGetCount:
    category: core
    doc_description: |
      Get the number of audio devices on the system.
    params:
      destination: Contains the number of audio devices on the system after a call to this function.
  Lav_deviceGetName:
    category: core
    doc_description: |
      Returns a human-readable name for the specified audio device.
    params:
      index: The index of the audio device.
      destination: Contains a pointer to  a string allocated by Libaudioverse containing the name. Use Lav_free on this string when done with it.
  Lav_deviceGetChannels:
    category: core
    doc_description: |
      Query the maximum number of channels for this device before downmixing occurs.
      You should query the user as to the type of audio they want rather than relying on this function.
      Some operating systems and backends will perform their own downmixing and happily claim 8-channel audio on stereo headphones.
      Furthermore, some hardware and device drivers will do the same.
      It is not possible for Libaudioverse to detect this case.
    params:
      index: The index of the audio device.
      destination: Contains the number of channels the device claims to support.
  Lav_createSimulation:
    category: simulations
    doc_description: |
      Creates a  simulation.
      The new simulation has no associated audio device.
      To make it output, use `Lav_simulationSetOutputDevice`.
    params:
      sr: The sampling rate of the new simulation.
      blockSize: The block size of the new simulation.
      destination: After a call to this function, contains the handle of the newly created simulation.
  Lav_simulationGetBlockSize:
    category: simulations
    doc_description: |
      Query the block size of the specified simulation.
    params:
      simulationHandle: The handle of the simulation.
      destination: The block size of the specified simulation.
  Lav_simulationGetBlock:
    category: simulations
    doc_description: |
      Gets a block of audio from the simulation and advances its time.
      using this with a simulation that is outputting audio to an audio device will not work well.
      
      You must allocate enough space to hold exactly one block of audio: the simulation's block size times the number of channels requested floating point values.
    params:
      simulationHandle: The handle of the simulation to read a block from.
      channels: The number of channels we want. The simulations' output will be upmixed or downmixed as appropriate.
      mayApplyMixingMatrix: If 0, drop any additional channels in the simulation's output and set any  missing channels in the simulation's output to 0. Otherwise, if we can, apply a mixing matrix.
      buffer: The memory to which to write the result.
  Lav_simulationGetSr:
    category: simulations
    doc_description: |
      Query the simulation's sampling rate.
    params:
      simulationHandle: The handle of the simulation.
      destination: Contains the sampling rate, in hertz.
  Lav_simulationSetOutputDevice:
    category: simulations
    doc_description: |
      Set the output device of the simulation.
      -1 is the system default.  0 and above are specific audio devices, matching the indices returned by the enumeration functions.
      
      Note that it is possible to change the output device of a simulation even after it has been set.
      This latter case is an expensive operation: Libaudioverse will block until all queued blocks have finished playing.
      
      After the output device has been set, calls to `Lav_simulationGetBlock` will error.
    params:
      simulationHandle: The simulation whose output device is to be set.
      index: The output device  the simulation is to play on.
      channels: The number of channels we wish to output.
      mixahead: The mixahead.  See the basics section for information.
  Lav_simulationClearOutputDevice:
    category: simulations
    doc_description: |
      Clear a simulation's output device.
      
      This is no-op if no output device has been set.
      If it has, this function will block until queued audio finishes.
      
      After a call to this function, it is again safe to use `Lav_simulationGetBlock`.
    params:
      simulationHandle: The simulation whose output device should be cleared.
  Lav_simulationLock:
    category: simulations
    doc_description: |
      All operations between a call to this function and a call to `Lav_simulationUnlock` will happen together, with no blocks mixed between them.
      This is equivalent to assuming that the simulation is a lock, with  all of the required caution that implies.
      If you do not call `Lav_simulationUnlock` in a timely manner, then audio will stop until you do.
      accessing another simulation for which your app uses `Lav_simulationLock` or
      locking or unlocking another lock are both  dangerous operations.
      Blocking will cause glitches in audio, but is otherwise safe.
    params:
      simulationHandle: The simulation to lock.
  Lav_simulationUnlock:
    category: simulations
    doc_description: |
      Release the internal lock of a simulation, allowing normal operation to resume.
      This is to be used after a call to `Lav_simulationLock` and on the same thread as that call; calling it in any other circumstance or on any other thread invokes undefined behavior.
    params:
      simulationHandle: The simulation to release.
  Lav_simulationSetBlockCallback:
    category: simulations
    doc_description: |
      Set a callback to be called just before every block and in the audio thread.
      This callback can and should access the Libaudioverse API:
      the point of it is that you can use it to perform tasks where missing even one block would be problematic, i.e. very precise scheduling of events.
      
      This  callback can even block, though this will slow down audio mixing and may cause glitchy audio.
      The one thing you should never do in this callback is access anything belonging to another simulation, as this can cause deadlock.
      
      The callback receives two parameters: the simulation to which it is associated and the time in simulation time that corresponds to the beginning of the block about to be mixed.
    params:
      simulationHandle: The simulation to set the callback for.
      callback: The callback to use.
      userdata: An extra parameter that will be passed to the callback.
  Lav_createBuffer:
    category: buffers
    doc_description: |
      Create an empty buffer.
    params:
      simulationHandle: The simulation to which this buffer belongs.
      destination: After a call to this function, contains the handle of the newly created buffer.
  Lav_bufferGetSimulation:
    category: buffers
    doc_description: |
      Get the handle of the simulation used to create this buffer.
    params:
      bufferHandle: The handle of the buffer.
      destination: After a call to this function, contains the simulation's handle.
  Lav_bufferLoadFromFile:
    category: buffers
    doc_description:
      Loads data into this buffer from a file.
      The file will be resampled to the sampling rate of the simulation.
      This will happen synchronously.
    params:
      bufferHandle: The buffer into which to load data.
      path: The path to the file to load data from.
  Lav_bufferLoadFromArray:
    category: buffers
    doc_description: |
      Load data into the specified buffer from the specified array of floating point audio data.
    params:
      bufferHandle: The buffer to load data into.
      sr: The sampling rate of the data in the array.
      channels: The number of audio channels in the data; frames*channels is the total length of the array in samples.
      frames: The number of frames of audio data; frames*channels is the length of the array in samples.
      data: A pointer to the beginning of the array to load from.
  Lav_nodeGetSimulation:
    category: nodes
    doc_description: |
      Get the simulation that a node belongs to.
    params:
      nodeHandle: The node to query.
      destination: After a call to this function, contains the handle to the simulation this node was created with.
  Lav_nodeConnect:
    category: nodes
    doc_description: |
      Connect the specified output of the specified node to the specified input of the specified node.
      
      it is an error if this would cause a cycle in the graph of nodes.
    params:
      nodeHandle: The node whose output we are going to connect.
      output: The index of the output to connect.
      destHandle: The node to whose input we are connecting.
      input: The input to which to connect.
  Lav_nodeConnectSimulation:
    category: nodes
    doc_description: |
      Connect the specified output of the specified node to the simulation's input.
    params:
      nodeHandle: The node to connect.
      output: The index of the output to connect.
  Lav_nodeConnectProperty:
    category: nodes
    doc_description: |
      Connect a node's output to an automatable property.
    params:
      nodeHandle: the node to connect.
      output: The output to connect.
      otherHandle: The node to which we are connecting.
      slot: The index of the property to which to connect.
  Lav_nodeDisconnect:
    category: nodes
    doc_description: |
      Disconnect the output of the specified node from all inputs to which it is connected.
    params:
      nodeHandle: The node to disconnect.
      output: The output to disconnect.
  Lav_nodeGetInputConnectionCount:
    category: nodes
    doc_description: |
      Get the number of inputs this node has.
    params:
      nodeHandle: The handle of the node.
      destination: After a call to this function, contains the number of inputs.
  Lav_nodeGetOutputConnectionCount:
    category: nodes
    doc_description: |
      Get the number of outputs this node has.
    params:
      nodeHandle: The node's handle.
      destination: After a call to this function, contains the number of outputs.
  Lav_nodeResetProperty:
    category: nodes
    doc_description: |
      Reset a property to its default.
    params:
      nodeHandle: The handle of the node.
      slot: The slot of the property to be reset.
  Lav_nodeSetIntProperty:
    category: nodes
    doc_description: |
      Set an int property.
      Note that this function also applies to boolean properties, as these are actually int properties with the range [0, 1].
    params:
      nodeHandle: The handle of the node.
      slot: The slot of the property on the specified node.
      value: The new value of the property.
  Lav_nodeSetFloatProperty:
    category: nodes
    doc_description: |
      Set the specified float property.
    params:
      nodeHandle: The node to manipulate.
      slot: The property to manipulate.
      value: the new value of the property.
  Lav_nodeSetDoubleProperty:
    category: nodes
    doc_description: |
      Set the specified double property.
    params:
      nodeHandle: The node to manipulate.
      slot: The property to manipulate.
      value: the new value of the property.
  Lav_nodeSetStringProperty:
    category: nodes
    doc_description: |
      Set the specified string property.
    params:
      nodeHandle: The node to manipulate.
      slot: The property to manipulate.
      value: the new value of the property.  Note that the specified string is copied and the memory may be freed.
  Lav_nodeSetFloat3Property:
    category: nodes
    doc_description: |
      Set the specified float3 property.
    params:
      nodeHandle: The node to manipulate.
      slot: The property to manipulate.
      v1: The first component of the float3.
      v2: The second component of the float3.
      v3: The third component of the float3.
  Lav_nodeSetFloat6Property:
    category: nodes
    doc_description: |
      Set the specified float6 property.
    params:
      nodeHandle: The node to manipulate.
      slot: The property to manipulate.
      v1: The first component of the float6.
      v2: The second component of the float6.
      v3: The third component of the float6.
      v4: The fourth component of the float6.
      v5: The fifth component of the float6.
      v6: The 6th component of the float6.
  Lav_nodeGetIntProperty:
    category: nodes
    doc_description: |
      Get the value of the specified int property.
    params:
      nodeHandle: The node that has the property.
      slot: The index of the property.
      destination: After a call to this function, contains the value of the property.
  Lav_nodeGetFloatProperty:
    category: nodes
    doc_description: |
      Get the specified float property's value.
    params:
      nodeHandle: The node that has the property.
      slot: The index of the property.
      destination: After a call to this function, contains the value of the property.
  Lav_nodeGetDoubleProperty:
    category: nodes
    doc_description: |
      Get the specified double property.
    params:
      nodeHandle: The node that has the property.
      slot: The index of the property.
      destination: After a call to this function, contains the value of the property.
  Lav_nodeGetStringProperty:
    category: nodes
    doc_description: |
      Get the specified string property.
    params:
      nodeHandle: The node that has the property.
      slot: The index of the property.
      destination: After a call to this function, contains a pointer to a newly allocated string that is a copy of the value of the property.  Free this string with Lav_free.
  Lav_nodeGetIntPropertyRange:
    category: nodes
    doc_description: |
      Get the range of an int property.
      Note that ranges are meaningless for  read-only properties.
    params:
      nodeHandle: the node.
      slot: The index of the property.
      destinationMin: After a call to this function, holds the range's minimum.
      destinationMax: After a call to this function, holds the range's maximum.
  Lav_nodeGetFloatPropertyRange:
    category: nodes
    doc_description: |
      Get the range of a float property.
      Note that ranges are meaningless for read-only properties.
    params:
      nodeHandle: the node.
      slot: The index of the property.
      destinationMin: After a call to this function, holds the range's minimum.
      destinationMax: After a call to this function, holds the range's maximum.
  Lav_nodeGetDoublePropertyRange:
    category: nodes
    doc_description: |
      Query the range of a double property.
      Note that ranges are meaningless for read-only properties.
    params:
      nodeHandle: the node.
      slot: The index of the property.
      destinationMin: After a call to this function, holds the range's minimum.
      destinationMax: After a call to this function, holds the range's maximum.
  Lav_nodeGetPropertyName:
    category: nodes
    doc_description: |
      Get the name of a property.
    params:
      nodeHandle: The node which ahs the property of interest.
      slot: The index of the property of interest.
      destination: After a call to this function, contains a newly allocated string that should be freed with Lav_free.  The string is the name of this property.
  Lav_nodeGetPropertyType:
    category: nodes
    doc_description: |
      Get the type of a property.
    params:
      nodeHandle: The node with the property of interest.
      slot: The index of the property of interest.
      destination: After a call to this function, contains the type of this property.
  Lav_nodeGetPropertyHasDynamicRange:
    category: nodes
    doc_description: |
      Find out whether or not a property has a dynamic range.
      Properties with dynamic ranges change their ranges at specified times, as documented by the documentation for the property of interrest.
    params:
      nodeHandle: The handle of the node with the property of interest.
      slot: The index of the property of interest.
      destination: After a call to this function, contains 1 if the property has a dynamic range, otherwise 0.
  Lav_nodeReplaceFloatArrayProperty:
    category: nodes
    doc_description: |
      Replace the array contained by a float array property with a new array.
      Note that, as usual, memory is copied, not shared.
    params:
      nodeHandle: The node  with the property of interest.
      slot: The index of the property of interest.
      length: The length of the new array.
      values: The array itself.
  Lav_nodeReadFloatArrayProperty:
    category: nodes
    doc_description: |
      Read the float array property at a specified index.
    params:
      nodeHandle: The node with the property of interest.
      slot: The index of the property of interest.
      index: The index at which to read.
      destination: After a call to this function, contains the value of the float array property at the specified index.
  Lav_nodeWriteFloatArrayProperty:
    category: nodes
    doc_description: |
      Write a range of values into the specified float array property, without changing its length.
    params:
      nodeHandle: The node with the property of interest.
      slot: The index of the property of interest.
      start: The starting index of the range to replace. Must be less than the length of the property.
      stop: One past the end of the region to be replaced. Must be no more than the length of the property.
      values: the data with which to replace the range. Must have length stop-start.
  Lav_nodeGetFloatArrayPropertyLength:
    category: nodes
    doc_description: |
      Get the length of the specified float array property.
    params:
      nodeHandle: The node with the property of interest.
      slot: The index of the property of interest.
      destination: After a call to this function, contains the current length of the property.
  Lav_nodeReplaceIntArrayProperty:
    category: nodes
    doc_description: |
      Replace the array contained by a int array property with a new array.
      Note that, as usual, memory is copied, not shared.
    params:
      nodeHandle: The node  with the property of interest.
      slot: The index of the property of interest.
      length: The length of the new array.
      values: The array itself.
  Lav_nodeReadIntArrayProperty:
    category: nodes
    doc_description: |
      Read the int array property at a specified index.
    params:
      nodeHandle: The node with the property of interest.
      slot: The index of the property of interest.
      index: The index at which to read.
      destination: After a call to this function, contains the value of the int array property at the specified index.
  Lav_nodeWriteIntArrayProperty:
    category: nodes
    doc_description: |
      Write a range of values into the specified  int array property, without changing its length.
    params:
      nodeHandle: The node with the property of interest.
      slot: The index of the property of interest.
      start: The starting index of the range to replace. Must be less than the length of the property.
      stop: One past the end of the region to be replaced. Must be no more than the length of the property.
      values: the data with which to replace the range. Must have length stop-start.
  Lav_nodeGetIntArrayPropertyLength:
    category: nodes
    doc_description: |
      Get the length of the specified int array property.
    params:
      nodeHandle: The node with the property of interest.
      slot: The index of the property of interest.
      destination: After a call to this function, contains the current length of the property.
  Lav_nodeGetArrayPropertyLengthRange:
    category: nodes
    doc_description: |
      Get the allowed range for the length of an array in an array property.
      This works on both int and float properties.
    params:
      nodeHandle: The node with the property of interest.
      slot: The index of the property of interest.
      destinationMin: After a call to this function, contains the minimum allowed length.
      destinationMax: After a call to this function, contains the maximum allowed length.
  Lav_nodeSetBufferProperty:
    category: nodes
    doc_description: |
      Set a buffer property.
    params:
      nodeHandle: The handle of the node with the property of interest.
      slot: The index of the property of interest.
      bufferHandle: The buffer to set the property to.  0 means none.
  Lav_nodeGetBufferProperty:
    category: nodes
    doc_description: |
      Gets the value of a specified buffer property.
    params:
      nodeHandle: The handle of the node with the property of interest.
      slot: The index of the property of interest.
      destination: After a call to this function, contains the handle of the buffer. Note that 0 means none.
  Lav_automationCancelAutomators:
    category: nodes
    doc_description: |
      Cancel all automators that are scheduled to begin running after the specified time.
    params:
      nodeHandle: The handle of the node with the property on which to cancel automators.
      slot: The index of the property to cancel automators on.
      time: The time after which to cancel automation.  This is relative to the node.
  Lav_automationLinearRampToValue:
    category: nodes
    doc_description: |
      Sets up a linear ramp.
      
      The value of a linear ramp begins at the end of the last automation and linearly increases to the start time of this automator, after which the property holds steady unless more automators are scheduled.
    params:
      nodeHandle: The node with the property to automate.
      slot: The index of the property to automate.
      time: The time at which we must be at the specified value.
      value: The value we must arrive at by the specified time.
  Lav_nodeGetEventHandler:
    category: nodes
    doc_description: |
      Gets an event handler for a specified event.
    params:
      nodeHandle: The handle of the node with the event to query.
      event: The index of the event.
      destination: After a call to this function, contains the address of the associated handler if any, otherwise NULL.
  Lav_nodeGetEventUserDataPointer:
    category: nodes
    doc_description: |
      Get the userdata of the specified event, if any.
      When event handlers are registered, the userdata is an extra parameter which is passed to them when the event fires.
    params:
      nodeHandle: The node with the event of interest.
      event: The index of the event of interest.
      destination: After a call to this function, contains the value of the userdata pointer if any, otherwise NULL.
  Lav_nodeSetEvent:
    category: nodes
    doc_description: |
      Set an event handler.
    params:
      nodeHandle: The node on which to set the handler.
      event: The event's index.
      handler: The function to use as the event handler.
      userData: An extra parameter which is passed to the handler.
  Lav_nodeReset:
    category: nodes
    doc_description: |
      Reset a node.
      What this means depends on the node in question, so see its documentation.
    params:
      nodeHandle: The node to reset.
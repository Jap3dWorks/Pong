---
description: '#include "Pong/core/reg_id_manager.h"'
---

# Register

## RegIdManager

Each RegIdManager is a collection of unique Ids (RegId), RegIdManager can create unique Ids.

## RegId

RegId is the key for al RegId maps, in case of components RegId represents the actor,

And all components with the same RegId are the components of the actor.

## Engine Reg types

<img src="../.gitbook/assets/file.drawing (3).svg" alt="" class="gitbook-drawing">

<img src="../.gitbook/assets/file.drawing.svg" alt="" class="gitbook-drawing">

## RegId Buffers and RegId file data

Some RegId buffers will contain all element datas, this is the case of buffered maps inside a Map like components.

Other RegId buffers will be removed once they will be submitted to the graphic card, this is the case of static meshes.

And Other buffer maps will contains the location of the data in the file system. So if a chunk of data is required it can be rapidly accessed. This is the case of static meshes not loaded yet.

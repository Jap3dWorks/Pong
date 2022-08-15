---
description: serialization
---

# File Data

file\_data is the serializer system, also includes serializable structs that can represent geometries and nodes.

_Needs real testing for write and for read data._

__

## serial\_data.h

Here is the the most high level functions,&#x20;

### Create serializer for custom objects

Override function&#x20;

```
template<typename Archive>
Pong::serializer::serialize(Archive& ar, Custom custom, Pong::serializer::Version version) {
   ar & custom.attr_1;
   ar * custom.attr_2;
}

```

## serialization.h


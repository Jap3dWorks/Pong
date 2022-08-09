# Map System

Map system is a scheme of classes and templates with the porpouse of describing a map.

As requisite we should achieve a compact layout data and a fast access elements (using SparseSet).

We will Define some concept elements:

* **RegId**, RegId is an int dressed of enum class, and its mission is to create a relation between multiple SparseSets, the way that a same RegId between multiple sets represents a whole (actor (refId) and components(sets elements)). RegId Should be managed using **RegIdManager**.
* **RegData**, RegData represents conjunction of related classes data. RegData manages each class separately using a SparseSets system (TypeRegMap). E.G all components should be included in the same RegData. Also RegData use RegId for elements conjunction.
*

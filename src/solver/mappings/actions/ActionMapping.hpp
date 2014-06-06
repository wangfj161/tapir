#ifndef SOLVER_ACTIONMAPPING_HPP_
#define SOLVER_ACTIONMAPPING_HPP_

#include "solver/abstract-problem/Action.hpp"

#include "ActionMappingEntry.hpp"

namespace solver {
class ActionMappingEntry;
class ActionNode;
class BeliefNode;

class ActionMapping {
public:
    ActionMapping() = default;
    virtual ~ActionMapping() = default;

    /* -------------- Association with a belief node ---------------- */
    /* Associates this mapping with the given belief node. */
    virtual void setOwner(BeliefNode *owner) = 0;
    /** Returns the belief node that owns this mapping. */
    virtual BeliefNode *getOwner() const = 0;
    /** Initializes this mapping (with respect to the belief node that owns it)
     *
     * This is optional; it can allow initialization of mappings based on
     * parameters of the given node, including the history of actions and
     * observations.
     *
     * This method is called when creating a new belief node, but is not
     * called when a belief node is deserialized, as the relevant parameters
     * can simply be stored for serialization.
     */
    virtual void initialize() {
    }

    /* -------------- Creation and retrieval of nodes. ---------------- */
    /** Retrieves the action node (if any) corresponding to this action. */
    virtual ActionNode *getActionNode(Action const &action) const = 0;
    /** Creates a new action node for the given action. */
    virtual ActionNode *createActionNode(Action const &action) = 0;
    /** Returns the number of child nodes associated with this mapping. */
    virtual long getNChildren() const = 0;

    /* -------------- Retrieval of mapping entries. ---------------- */
    /** Returns the number of entries in this mapping with a nonzero visit
     * count (some of these may not have an associated action node, so this
     * is different to the number of child nodes).
     */
    virtual long getNumberOfVisitedEntries() const = 0;
    /** Returns all of the visited entries in this mapping - some may have
     * null action nodes if the visit counts were initialized to nonzero
     * values.
     */
    virtual std::vector<ActionMappingEntry const *> getVisitedEntries() const = 0;

    /** Returns the mapping entry (if any) associated with the given action. */
    virtual ActionMappingEntry *getEntry(Action const &action) = 0;
    /** Returns the mapping entry (if any) associated with the given action. */
    virtual ActionMappingEntry const *getEntry(Action const &action) const = 0;

    /* ------------------ Methods for unvisited actions ------------------- */
    /** Returns true iff this mapping still has actions that remain to be tried. */
    virtual bool hasActionsToTry() const = 0;
    /** Returns the next action to be tried for this node. */
    virtual std::unique_ptr<Action> getNextActionToTry() const = 0;

    /* -------------- Retrieval of general statistics. ---------------- */
    /** Returns the total number of times children have been visited. */
    virtual long getTotalVisitCount() const = 0;

    /* --------------- Methods for updating the values ----------------- */
    /** Updates the given action, by adding the given number of visits and the
     * given change in the total q-value.
     *
     * Returns true if and only if the q value of the action changed.
     */
    virtual bool update(Action const &action, long deltaNVisits, double deltaTotalQ) {
        return getEntry(action)->update(deltaNVisits, deltaTotalQ);
    }
};

} /* namespace solver */

#endif /* SOLVER_ACTIONMAPPING_HPP_ */

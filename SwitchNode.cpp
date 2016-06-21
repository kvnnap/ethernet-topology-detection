//
// Created by kevin on 6/19/16.
//

#include "SwitchNode.h"
#include "EthernetFrame.h"

using namespace std;
using namespace Network;

SwitchNode::SwitchNode(SimulatedNetworkInterface& p_sim)
    : NetworkNode (SWITCH, p_sim)
{ }

void SwitchNode::receive(SimulationData& p_simData) {

    // set sim data
    NetworkNode * const from = p_simData.from;
    SimulationData simData = p_simData;
    simData.from = this;

    // Extract EthernetFrame
    EthernetFrame ef (simData.buffer);
    if (macPortMap.find(ef.sourceMac) == macPortMap.end()) {
        // not found - simply add it
        macPortMap.insert(make_pair(ef.sourceMac, from));
    }

    // We have it, check if macPortMap is consistent
    if (macPortMap[ef.sourceMac] != from) {
        macPortMap[ef.sourceMac] = from;
    }

    // Check if we know to whom to send this
    // We don't
    if (macPortMap.find(ef.destinationMac) == macPortMap.end()) {
        // Send to everyone except from source
        for (NetNodePt& child : getChildPeerNodes()) {
            if (from != child.get()) {
                simData.to = child.get();
                send(simData);
            }
        }
        for (NetworkNode * node : getParentPeerNodes()) {
            if (from != node) {
                simData.to = node;
                send(simData);
            }
        }
    } else {
        NetworkNode * node = macPortMap[ef.destinationMac]; //macPortMap.find(ef.destinationMac)->second;
        if (from != node) {
            simData.to = node;
            send(simData);
        }
    }
}

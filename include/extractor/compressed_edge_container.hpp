#ifndef GEOMETRY_COMPRESSOR_HPP_
#define GEOMETRY_COMPRESSOR_HPP_

#include "util/payload.hpp"
#include "util/typedefs.hpp"

#include <unordered_map>

#include <string>
#include <vector>

namespace osrm
{
namespace extractor
{

class CompressedEdgeContainer
{
  public:
    struct OnewayCompressedEdge
    {
      public:
        NodeID node_id;                  // refers to an internal node-based-node
        EdgeWeight weight;               // the weight of the edge leading to this node
        UncompressedEdgePayload payload; // the payload of the edge leading to this node
    };

    using OnewayEdgeBucket = std::vector<OnewayCompressedEdge>;

    CompressedEdgeContainer();
    void CompressEdge(const EdgeID surviving_edge_id,
                      const EdgeID removed_edge_id,
                      const NodeID via_node_id,
                      const NodeID target_node,
                      const EdgeWeight weight1,
                      const EdgeWeight weight2,
                      const UncompressedEdgePayload &payload1,
                      const UncompressedEdgePayload &payload2);

    void AddUncompressedEdge(const EdgeID edge_id,
                             const NodeID target_node,
                             const EdgeWeight weight,
                             const UncompressedEdgePayload &payload);

    void InitializeBothwayVector();
    unsigned ZipEdges(const unsigned f_edge_pos, const unsigned r_edge_pos);

    bool HasEntryForID(const EdgeID edge_id) const;
    bool HasZippedEntryForForwardID(const EdgeID edge_id) const;
    bool HasZippedEntryForReverseID(const EdgeID edge_id) const;
    void PrintStatistics() const;
    void SerializeInternalVector(const std::string &path) const;
    unsigned GetPositionForID(const EdgeID edge_id) const;
    unsigned GetZippedPositionForForwardID(const EdgeID edge_id) const;
    unsigned GetZippedPositionForReverseID(const EdgeID edge_id) const;
    const OnewayEdgeBucket &GetBucketReference(const EdgeID edge_id) const;
    bool IsTrivial(const EdgeID edge_id) const;
    NodeID GetFirstEdgeTargetID(const EdgeID edge_id) const;
    NodeID GetLastEdgeTargetID(const EdgeID edge_id) const;
    NodeID GetLastEdgeSourceID(const EdgeID edge_id) const;

  private:
    int free_list_maximum = 0;

    void IncreaseFreeList();
    std::vector<OnewayEdgeBucket> m_compressed_oneway_geometries;
    std::vector<unsigned> m_compressed_geometry_index;
    std::vector<NodeID> m_compressed_geometry_nodes;
    std::vector<EdgeWeight> m_compressed_geometry_fwd_weights;
    std::vector<EdgeWeight> m_compressed_geometry_rev_weights;
    std::vector<EdgeWeight> m_compressed_geometry_fwd_durations;
    std::vector<EdgeWeight> m_compressed_geometry_rev_durations;
    std::vector<unsigned> m_free_list;
    std::unordered_map<EdgeID, unsigned> m_edge_id_to_list_index_map;
    std::unordered_map<EdgeID, unsigned> m_forward_edge_id_to_zipped_index_map;
    std::unordered_map<EdgeID, unsigned> m_reverse_edge_id_to_zipped_index_map;
};
}
}

#endif // GEOMETRY_COMPRESSOR_HPP_

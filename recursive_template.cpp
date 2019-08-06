#include <unordered_map>
#include <variant>
#include <vector>

struct node {
  using value_type = std::variant<int>;
  value_type m_payload;
};

template <typename Settings> struct __node_map : public node {
  std::unordered_map<std::string, typename Settings::children_type> m_children;
};

template <typename Settings>
struct __node_array : public node {
  std::vector<typename Settings::children_type> m_children;
};

template <typename Settings>
using __node_variant = std::variant<typename Settings::map_type,
                                    typename Settings::array_type, node>;

template <template <typename> typename Map,
          template <typename> typename Array,
          template <typename> typename Variant>
struct __node_settings {
  using map_type      = Map    <__node_settings>;
  using array_type    = Array  <__node_settings>;
  using children_type = Variant<__node_settings>;
};

using __settings_helper = __node_settings<__node_map, __node_array, __node_variant>;

using node_map   = __settings_helper::map_type;
using node_array = __settings_helper::array_type;

int main() {
  node_map m;

  m.m_children["primitive"] = node{};
  m.m_children["array"] = node_array{};
  std::get<node_array>(m.m_children["array"]).m_children.push_back(node{});
  std::get<node_array>(m.m_children["array"]).m_children.push_back(node_map{});
  std::get<node_map>(std::get<node_array>(m.m_children["array"]).m_children[1])
      .m_children["primitive"] = node{};
}

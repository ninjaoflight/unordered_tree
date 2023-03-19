#pragma once
#include <algorithm>
#include <memory>
#include <string>
#include <unordered_tree/scalar_value.hpp>

class Node;
class NodeValue;

// class that helps to contain a Node within a NodeValue
class NodeVariant {
	// the node cannot be null
	std::unique_ptr<Node> value;

  public:
	NodeVariant();
	NodeVariant(NodeVariant const &other);
	NodeVariant(NodeVariant &&other);
	NodeVariant(Node const &node);
	NodeVariant(Node &&node);

	std::reference_wrapper<Node> get_value() const;

	NodeVariant &operator=(NodeVariant const &other);
	NodeVariant &operator=(NodeVariant &&other) noexcept;
};

/*
 * helper class that can contain a scalar value or a node within
 */
class NodeValue {
	using node_t = std::variant<ScalarValue, NodeVariant>;
	node_t value = ScalarValue{};

  public:
	NodeValue() = default;
	// explicit NodeValue(node_t value) : value(std::move(value)) {}
	NodeValue(NodeValue const &other);
	NodeValue(NodeValue &&other) noexcept;
	NodeValue(Node &&value);
	NodeValue(ScalarValue scalar);
	template <typename T>
	    requires(DecaysToScalarType<T>)
	NodeValue(T scalar) : NodeValue(ScalarValue{scalar}){};

	bool is_scalar() const;
	bool is_node() const;

	std::optional<ScalarValue> as_scalar() const;

	std::optional<std::reference_wrapper<Node>> as_node() const;

	template <typename T> constexpr bool is() const {
		if constexpr (std::is_same_v<T, ScalarValue>) {
			return is_scalar();
		} else if constexpr (std::is_same_v<T, Node>) {
			return is_node();
		} else {
			auto value = as_scalar();
			return value && value->is<T>();
		}
	}

	NodeValue &operator=(NodeValue const &other) = default;
	NodeValue &operator=(NodeValue &&other) noexcept = default;
};

class Node {
  public:
	Node() = default;
	Node(Node const &other) noexcept = default;
	Node(Node &&other) = default;
	Node &operator=(Node const &other) noexcept = default;
	Node &operator=(Node &&other) = default;
};

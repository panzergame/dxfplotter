import xml.etree.ElementTree as ET
from jinja2 import Environment, FileSystemLoader
from sys import argv


class Node:
	count = 0

	def __init__(self, name):
		# Split name in tokens
		tokens = name.split(" ")
		# All tokens titled (first letter capitalized)
		titled_tokens = [*map(lambda e: e.title(), tokens)]

		# thisIsAName
		self.var_name = "".join((tokens[0], *titled_tokens[1:]))
		# This Is A Name
		self.name = " ".join(titled_tokens)
		self.description = ""
		# ThisIsAName
		self.class_name = "".join(titled_tokens)

		self.unique_class_name = "{}{}".format(self.class_name, Node.count)
		Node.count += 1

	def __repr__(self):
		return self.unique_class_name


class Group(Node):
	def __init__(self, name, children):
		super().__init__(name)

		self.children = children

		self.tuple_name = ", ".join(map(lambda p: p.unique_class_name, children))


class List(Node):
	def __init__(self, name, item):
		super().__init__(name)

		self.item = item


class Property(Node):
	def __init__(self, name, base_type, default_value):
		super().__init__(name)

		# Final type, mainly std::string, float or int
		self.base_type = base_type
		# Default value in case of empty configuration file
		self.default_value = default_value

		if self.base_type == "std::string":
			self.default_value = '"' + self.default_value + '"'

		self.class_name = "Property<{}>".format(self.base_type)
		self.unique_class_name = self.class_name


def load_from_node(xml_node, nodes):
	tag = xml_node.tag
	name = xml_node.attrib["name"]

	if tag == "group":
		children = [load_from_node(n, nodes) for n in xml_node]
		node = Group(name, children)
	elif tag == "list":
		item = load_from_node(xml_node[0], nodes)
		node = List(name, item)
	elif tag == "property":
		# Parse additional information
		base_type = xml_node.attrib["type"]
		default_value = xml_node.attrib["default"]
		node = Property(name, base_type, default_value)
	else:
		raise(ValueError("Node tag not implemented"))

	# Track all nodes in flat list
	nodes.append(node)

	return node


def load_from_xml(file_name):
	tree = ET.parse(file_name)

	nodes = []
	root = load_from_node(tree.getroot(), nodes)

	return root, nodes


propertie_file_name = argv[1]
template_dir_name = argv[2]
template_file_name = argv[3]
output_file_name = argv[4]

root, nodes = load_from_xml(propertie_file_name)

file_loader = FileSystemLoader(template_dir_name)
env = Environment(loader=file_loader)

template = env.get_template(template_file_name)
template.stream(root=root, nodes=nodes, Property=Property, Group=Group, List=List).dump(output_file_name)

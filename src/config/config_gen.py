import xml.etree.ElementTree as ET
from jinja2 import Environment, FileSystemLoader
from sys import argv

class NameVariation:
	def __init__(self, name):
		# Split name in tokens
		tokens = name.split(" ")
		# All tokens titled (first letter capitalized)
		titled_tokens = [token.title() for token in tokens]

		# thisIsAName
		self.cpp_name = "".join((tokens[0], *(token for token in titled_tokens[1:])))
		# this_is_a_name
		self.ini_name = "_".join(tokens)
		# This Is A Name
		self.description = " ".join(titled_tokens)
		# ThisIsAName
		self.class_name = "".join(titled_tokens)

class Section(NameVariation):
	def __init__(self, name, properties):
		super().__init__(name)
		# Properties of the section
		self.properties = properties

class Property(NameVariation):
	def __init__(self, name, base_type, default_value):
		super().__init__(name)

		# Final type, mainly std::string, float or int
		self.base_type = base_type
		# Default value in case of empty configuration file
		self.default_value = default_value

def section_properties_from_xml(section):
	properties = [Property(prop.attrib["name"], prop.attrib["type"], prop.text) for prop in section]

	return properties

def load_sections_from_xml(file_name):
	tree = ET.parse(file_name)
	root = tree.getroot()

	sections = [Section(section.attrib["name"], section_properties_from_xml(section)) for section in root]

	return sections


propertie_file_name = argv[1]
template_dir_name = argv[2]
template_file_name = argv[3]
output_file_name = argv[4]

sections = load_sections_from_xml(propertie_file_name)

file_loader = FileSystemLoader(template_dir_name)
env = Environment(loader=file_loader)

template = env.get_template(template_file_name)
template.stream(sections=sections).dump(output_file_name)

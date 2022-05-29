add_requires("glib >=2.71.0")

target("console")
	set_kind("binary")
	add_files("*.c")
	add_packages("glib")

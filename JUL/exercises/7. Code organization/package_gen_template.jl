using PkgTemplates

template = Template(;
    user = "ezvezdov",            # github user name
    authors = ["Yauheni Zviazdou"],   # list of authors
    dir = ".",              # dir in which the package will be created
    julia = v"1.8",                     # compat version of Julia
    plugins = [
        !CompatHelper,                  # disable CompatHelper
        !TagBot,                        # disable TagBot
        Readme(; inline_badges = true), # added readme file with badges
        Tests(; project = true),        # added Project.toml file for unit tests
        Git(; manifest = false),        # add manifest.toml to .gitignore
        License(; name = "MIT")         # addedMIT licence
    ],
)
template("ImageInspector")
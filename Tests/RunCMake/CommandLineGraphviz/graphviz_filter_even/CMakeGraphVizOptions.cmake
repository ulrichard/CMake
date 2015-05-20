set(GRAPHVIZ_EXTERNAL_LIBS ON)
set(GRAPHVIZ_GENERATE_PER_TARGET FALSE)
set(GRAPHVIZ_GENERATE_DEPENDERS FALSE)
set(GRAPHVIZ_NODE_FILTER
    "([A-Z]:)?/?([^/]+/)*(lib)?([A-Za-z]+).*\\.(lib|so|dll|a)" "\\4"
    "([a-z]+)_.*" "\\1"
)

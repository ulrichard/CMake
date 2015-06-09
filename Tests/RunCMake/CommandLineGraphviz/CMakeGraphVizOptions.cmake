set(GRAPHVIZ_EXTERNAL_LIBS On)
set(GRAPHVIZ_NODE_FILTER
    "([A-Z]:)?/([^/]+/)*(lib)?([A-Za-z]+).*\\.(lib|so|dll|a)" "\\4"
)

{
    "targets": [
        {
            "target_name": "mouse-forward-back",
            'include_dirs': [
                '<!(node -e "require(\'nan\')")'
            ],
            "libraries": ['-lX11'],
            "sources": [ "mouse-forward-back.cc" ]
        }
    ]
}

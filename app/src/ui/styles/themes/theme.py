from pathlib import Path

# Define color palettes
light_colors = {
    "bg": "#FAFAFA",
    "surface": "#F0EAF7",
    "text": "#333333",
    "border": "#DDDDDD",
    "accent": "#9E7FC3"
}

dark_colors = {
    "bg": "#1F1B24",
    "surface": "#2D2533",
    "text": "#EAEAEA",
    "border": "#3C3442",
    "accent": "#D5C6F0"
}

# Base template with placeholders
template = """
QWidget {{
    background-color: {bg};
    color: {text};
}}

QLineEdit {{
    background-color: {surface};
    border: 1px solid {border};
    color: {text};
    padding: 6px;
    border-radius: 6px;
}}

QPushButton {{
    background-color: {accent};
    color: white;
    border: none;
    padding: 6px 12px;
    border-radius: 6px;
}}

QPushButton:hover {{
    background-color: {border};
    color: {text};
}}
"""

# Write themed stylesheets
output_dir = Path("")
output_dir.mkdir(parents=True, exist_ok=True)

light_qss = template.format(**light_colors)
dark_qss = template.format(**dark_colors)

light_path = output_dir / "light.qss"
dark_path = output_dir / "dark.qss"

light_path.write_text(light_qss)
dark_path.write_text(dark_qss)

light_path, dark_path

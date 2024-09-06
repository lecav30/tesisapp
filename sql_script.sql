DROP TABLE IF EXISTS {{ name }};
CREATE TABLE {{ name }} (
    {{ name }}_id INT NOT NULL AUTO_INCREMENT,
    {% for column in columns %}
    {{ column.name }} {{ column.type }} {{ column.null }},
    {% endfor %}
    PRIMARY KEY ({{ name }}_id)
);

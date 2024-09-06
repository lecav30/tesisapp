DROP TABLE IF EXISTS {{ name }};
CREATE TABLE {{ name }} (
    {{ name }}_id INT NOT NULL AUTO_INCREMENT,
    {% for column in columns %}
    {{ column.name }} {{ column.type }} {{ column.null }},
    {% endfor %}
    PRIMARY KEY ({{ name }}_id){% if length(foreign_keys) > 0 %},{% endif %}
    {% for foreign_key in foreign_keys %}
    FOREIGN KEY ({{ foreign_key.name }}) REFERENCES {{ foreign_key.table }}({{ foreign_key.name }}){% if not loop.is_last %},{% endif %}
    {% endfor %}
);

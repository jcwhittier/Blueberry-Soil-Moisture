CREATE DATABASE "BlueberrySoilMoisture"
    WITH 
    OWNER = postgres
    ENCODING = 'UTF8'
    LC_COLLATE = 'en_US.UTF-8'
    LC_CTYPE = 'en_US.UTF-8'
    TABLESPACE = pg_default
    CONNECTION LIMIT = -1;



CREATE TABLE public.observations
(
    observation_id integer NOT NULL DEFAULT nextval('observations_observation_id_seq'::regclass),
    sensor_id integer NOT NULL,
    observation_time timestamp with time zone,
    gateway_time timestamp with time zone,
    insert_time timestamp with time zone DEFAULT now(),
    value_decimal numeric(8,4),
    value_string character varying(32) COLLATE pg_catalog."default",
    CONSTRAINT observations_pkey PRIMARY KEY (observation_id)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE public.observations
    OWNER to postgres;

GRANT ALL ON TABLE public.observations TO postgres;

GRANT INSERT, SELECT ON TABLE public.observations TO blueberry;
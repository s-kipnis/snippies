// emitting yaml
    fn dump(doc: &yaml_rust::Yaml) {
        let mut out_str = String::new();
        let mut emitter = yaml_rust::YamlEmitter::new(&mut out_str);
        emitter.dump(doc).unwrap(); // dump the YAML object to a String
        println!("{}", out_str);
    }

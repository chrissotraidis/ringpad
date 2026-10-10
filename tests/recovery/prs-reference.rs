// Offline cross-check wrapper for pinned prs-rs source. No dependency downloads.
// Compile with PRS_REFERENCE_SOURCE pointing at the inspected private source copy.
mod common { include!(concat!(env!("PRS_REFERENCE_SOURCE"), "/common.rs")); }
mod decoder { include!(concat!(env!("PRS_REFERENCE_SOURCE"), "/decompress.rs")); }
fn main() {
    let args: Vec<String> = std::env::args().collect();
    assert_eq!(args.len(), 4, "input output expected-decoded-size");
    let input = std::fs::read(&args[1]).unwrap();
    let size: usize = args[3].parse().unwrap();
    assert!(input.len() <= 16 * 1024 * 1024 && size <= 16 * 1024 * 1024);
    // Run only on inputs already validated by the bounded Python reader.
    let mut output = vec![0u8; size + 1024];
    let written = unsafe { decoder::prs_decompress(input.as_ptr(), output.as_mut_ptr()) };
    assert_eq!(written, size);
    let mut file = std::fs::OpenOptions::new().write(true).create_new(true).open(&args[2]).unwrap();
    std::io::Write::write_all(&mut file, &output[..written]).unwrap();
    println!("decoded {} bytes", written);
}

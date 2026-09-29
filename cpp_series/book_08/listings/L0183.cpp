#include <opentelemetry/sdk/trace/tracer_provider.h>
#include <opentelemetry/sdk/trace/simple_processor.h>
#include <opentelemetry/exporters/ostream/span_exporter.h>
#include <memory>
int main(){
 namespace sdk=opentelemetry::sdk::trace;
 auto exporter=std::make_unique<opentelemetry::exporter::trace::OStreamSpanExporter>();
 auto processor=std::make_unique<sdk::SimpleSpanProcessor>(std::move(exporter));
 auto provider=std::make_shared<sdk::TracerProvider>(std::move(processor));
 auto tracer=provider->GetTracer("harbor.section17","1.0");
 auto span=tracer->StartSpan("job.process");
 span->SetAttribute("job.kind","square");
 span->SetStatus(opentelemetry::trace::StatusCode::kOk);
 span->End();provider->ForceFlush();
}
